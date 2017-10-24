
--create type TranscriptRecord as (code text, term text, course integer, prog text, name text, mark integer, grade text, uoc integer, rank integer, totalEnrols integer);

create type TranscriptRecord as (code char(8), term char(4), course integer, 
  prog char(4), name text, mark integer, grade char(2), 
  uoc integer, rank integer, totalEnrols integer);

--convert semester to proper format
create or replace function
  convertSemester(integer) returns text
as
$$
select concat(substring(CAST(s.year as CHAR(4)), 3, 2) , lower(s.term))
from semesters s
where s.id = $1 
$$ language sql;

-- calculate total students enrolled in a course
create or replace function
  totalStudents(integer) returns bigint
as
$$
select count(student)
from course_enrolments
where course = $1 and mark is not null
$$ language sql;

-- finds the students rank
create or replace function
  rank(integer,integer) returns bigint
as
$$
select count(student)
from course_enrolments 
where course = $1 and mark > $2 and mark is not null
$$ language sql;

create or replace function rank(integer,integer) 
  returns bigint
as $$
Declare
  r record;
  rank integer;
Begin
rank:= 1;
for r in select student
from course_enrolments 
where course = $1 and mark > $2 and mark is not null
loop
rank:= rank + 1;
end loop; 
if $2 is null then
rank := null;
end if;
return rank; 
END;
$$ language plpgsql;


create or replace function
  uocCounter(char,integer) returns integer
as $$
declare 
  totalUoc integer;
begin
totalUoc := 6;
  if $1 != 'SY' and $1 != 'PC' and $1 != 'PS' and $1 != 'CR' and $1 != 'DN' and $1 != 'HD' and $1 != 'PT' and $1 != 'A' 
  and $1 != 'B' and $1 != 'C'  and $2 is not null then
    totalUoc := 0;
end if; 
return totalUoc;
end;   
$$ language plpgsql;

create or replace function Q1(integer)
  returns setof TranscriptRecord
as $$
declare
record TranscriptRecord;
begin
for record in 
select sub.code , convertSemester(c.semester), ce.course,prog.code
,sub.name, ce.mark, ce.grade,uocCounter(ce.grade,ce.mark),cast((rank(ce.course,ce.mark)) 
as integer),cast(totalStudents(ce.course) as integer)

from students s , people p , course_enrolments ce , 
courses c, subjects sub,program_enrolments pe, programs prog

where p.id = s.id and p.unswid = $1 and s.id = ce.student 
and c.id = ce.course and c.subject = sub.id 
and pe.student = ce.student and pe.program = prog.id

group by sub.code,c.semester,ce.course,prog.code
,sub.name, ce.mark, ce.grade,sub.uoc,c.subject

order by convertSemester(c.semester)
LOOP
return next record;
end loop;
end;

$$ language plpgsql;


-- Q2: ...
create type MatchingRecord as ("table" text, "column" text, nexamples integer);

create or replace function
  totalFound("table" text,"column" text,pattern text) returns integer
as
$$
Declare
  temp integer;
Begin
EXECUTE format('select count(*)::integer from %s where %s::text ~ ''%s''', $1, $2, $3) INTO temp;
return temp;
End;
$$ language plpgsql;

create or replace function Q2("table" text, pattern text) 
  returns setof MatchingRecord
as 
$$
Declare
  mr MatchingRecord;
  r text;
  temp integer;
Begin
  for r in select column_name from information_schema.columns where table_name = $1
  loop
  temp := totalFound($1,r,$2);
  if temp > 0 then  
  mr.table := $1;
  mr.column := r;
  mr.nexamples := temp;
  return next mr;
  end if;
  end loop;
END;
$$ 
language plpgsql;

create or replace function kappa(integer,integer) returns integer
as
$$
select count(p.unswid)::integer
from affiliations a, orgunit_groups og, staff_roles s, orgunits o, people p
where a.staff = p.id
and (a.orgunit = $1 or og.owner = $1)
and a.orgunit = og.member
and a.role = s.id
and a.orgunit = o.id
and p.unswid = $2
$$ language sql;

create or replace function potentialPeople(integer) returns table(sortname text ,unswid integer, name text,
    jobTitle text , faculty text,starting date, ending date) as
$$
select p.sortname, p.unswid, p.name, s.name, o.name, a.starting, a.ending
from affiliations a, orgunit_groups og, staff_roles s, orgunits o, people p
where a.staff = p.id
and (a.orgunit = $1 or og.owner = $1)
and a.orgunit = og.member
and a.role = s.id
and a.orgunit = o.id
and kappa($1,p.unswid) > 1
order by p.sortname,a.starting

$$ language sql;

create type peopleNeeded as (unswid integer, name text,sortname text 
  , roles text,starting date, ending date);
create or replace function peopleWeNeed(integer) 
  returns setof peopleNeeded
as $$
Declare
  er peopleNeeded;
  r record;
  fail integer;
  foundOne integer;
  tempZid integer;
  count integer;
Begin
fail := 0;
tempZid := 0;
count:= 0;
for r in select * from potentialPeople($1) pp order by pp.sortname , pp.starting 
loop
   
  if tempZid <> r.unswid then
    fail:= 0;
    tempZid := r.unswid;
    count:= 0;
  end if; 
 
  if r.ending is null and count = 0 then
    fail := 1;
  end if;

  if fail = 0 then
  er.unswid := r.unswid;
  er.name :=r.name;
  er.sortname := r.sortname;
  er.starting := r.starting;
  er.ending := r.ending;
  if r.ending is null then
  er.roles :=r.jobTitle||' '||r.faculty||' '||r.starting ||E'\n' ;
  else 
  er.roles :=r.jobTitle||' '||r.faculty  ||' '||r.starting||' '||r.ending ||E'\n';
  count := count + 1;
  end if;
  return next er;
  end if;

end loop;  
END;
$$ language plpgsql;



create type finalFiltered as (unswid integer, name text,sortname text ,roles text,test text);
create or replace function filterFinal(integer) 
  returns setof finalFiltered
as $$
Declare
  ff finalFiltered;
  r record; 
  initial integer;
  tempId integer;
  tempEnding date;
Begin

initial:=0;
-- tempEnding := null;

for r in select * from peopleWeNeed($1) pwn order by pwn.sortname , pwn.starting 
loop

  if initial = 0 then
    tempEnding:=r.ending;
    tempId := r.unswid;
    ff.unswid:=r.unswid;
    ff.name:=r.name;
    ff.sortname := r.sortname;
    ff.roles:=r.roles;
    ff.test := 'initial';
    initial := 1;
    return next ff;

  end if;


  if tempId = r.unswid and r.starting >= tempEnding  then
    
    ff.unswid:=r.unswid;
    ff.name:=r.name;
    ff.sortname := r.sortname;
    ff.roles:=r.roles; 
    -- tempId := r.unswid;
    ff.test := 'not first of a kind';
     
    if r.ending is not null then
        tempEnding := r.ending;
    else 
        tempId := 0;
    end if;

     return next ff;
  
  elsif tempId <> r.unswid then
    ff.test := 'head';
    ff.unswid:=r.unswid;
    ff.name:=r.name;
    ff.sortname := r.sortname;
    ff.roles:=r.roles;
    tempEnding:=r.ending;
    tempId := r.unswid;
    return next ff;
  end if;

  end loop;  
END;
$$ language plpgsql;

create or replace function preDistinctID(integer)
returns table(unswid integer,count integer)
as
$$
select unswid,count(unswid)::integer
from filterFinal($1)
group by unswid
$$language sql;


create or replace function distinctId(integer)
returns table(unswid integer)
as
$$
select unswid
from preDistinctID($1) 
where count >1
$$language sql;



create or replace function allPeople(integer) returns table(unswid integer, name text,sortname text,
    jobTitle text , faculty text,starting date, ending date) as
$$
select pp.unswid, pp.name,pp.sortname, pp.jobTitle, pp.faculty, pp.starting, pp.ending
from potentialPeople($1) pp , distinctId($1) ff
where pp.unswid = ff.unswid
order by pp.sortname,pp.starting;
$$ language sql;


create type concatString as (unswid integer, name text,sortname text, roles text);
create or replace function concatStrings(integer) 
  returns setof concatString
as $$
Declare
  er concatString;
  r record;
Begin
for r in select * from allPeople($1)
loop
  er.unswid := r.unswid;
  er.name :=r.name;
  er.sortname :=r.sortname;
  if r.ending is null then
  er.roles :=r.jobTitle||', '||r.faculty||' ('||r.starting ||'..)'||E'\n' ;
  else 
  er.roles :=r.jobTitle||', '||r.faculty  ||' ('||r.starting||'..'||r.ending ||')'|| E'\n';
  end if;
  return next er;
end loop;  
END;
$$ language plpgsql;




create type EmploymentRecord as (unswid integer, name text, roles text);
create or replace function Q3(integer) 
  returns setof EmploymentRecord 
as $$
Declare
  er EmploymentRecord;
  r record;
Begin
for r in select pwn.unswid , pwn.name , string_agg(pwn.roles,'' ) 
from concatStrings($1) pwn 
group by pwn.unswid,pwn.name,pwn.sortname
order by pwn.sortname
loop
return next r;
end loop;  
END;
$$ language plpgsql;


