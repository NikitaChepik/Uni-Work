-- COMP3311 16s1 Project 1
--
-- MyMyUNSW Solution Template


-- Q1: students who have taken more than 55 courses
create or replace view Q1(unswid, name)
as
select p.unswid,p.name
from People p , course_enrolments e
where p.id = e.student
group by p.id
having count(*) > 55
order by p.unswid
;


-- Q2: get details of the current Heads of Schools
create or replace view Q2(name, school, starting)
as
select p.name,u.longname, a.starting
from People p , affiliations a, staff_roles r,OrgUnits u,orgunit_types t
where a.staff = p.id and a.ending IS NULL and a.isprimary = 't' 
and r.id = a.role and r.name = 'Head of School' 
and u.id = a.orgunit and u.utype = t.id and  t.name = 'School'
;



-- Q3 UOC/ETFS ratio
create or replace view basic(ratio)
as
select (cast((s.uoc/s.eftsload) as numeric(4,1)))
from subjects s
where s.uoc > 0 and s.eftsload > 0;


create or replace view Q3(ratio,nsubjects)
as
select b.ratio, count(b.ratio)
from basic b
group by b.ratio
order by b.ratio;


-- -- Q4: convenor for the most courses
create or replace view findall(name, ncourses)
as
select p.name, count(*)
from people p, course_staff s, staff_roles r
where p.id = s.staff and s.role = r.id and r.name = 'Course Convenor'
group by p.id
;

create or replace view largest(largest)
as
select max(ncourses)
from findall;


create or replace view Q4(name, ncourses)
as
select fa.name , fa.ncourses
from findall fa, largest l
where fa.ncourses = l.largest;

-- Q5: program enrolments from 05S2
create or replace view Q5a(id)
as
select p.unswid
from program_enrolments e, people p, programs r, semesters s
where e.student = p.id and e.program = r.id and e.semester = s.id
and s.year = 2005 and s.term = 'S2' and r.code = '3978' 
;

create or replace view Q5b(id)
as
select p.unswid
from stream_enrolments se, program_enrolments pe, streams s, people p, semesters sem
where se.stream = s.id and se.partof = pe.id 
and pe.student = p.id and pe.semester = sem.id
and s.code = 'SENGA1' and sem.year = 2005 and sem.term = 'S2'
group by p.id
;


create or replace view Q5c(id)
as
select p.unswid
from program_enrolments pe, programs r, orgunits u, people p, semesters s
where pe.student = p.id and pe.semester = s.id
and pe.program = r.id and r.offeredby = u.id
and s.year = 2005 and s.term = 'S2' and u.unswid in ('COMPSC', 'ZCOM') 
group by p.id
order by p.unswid
;



-- Q6: semester names
-- Testing case in check.sql: SELECT * FROM Q6(123);
create or replace function
	Q6(integer) returns text
as
$$
select concat(substring(CAST(s.year as CHAR(4)), 3, 2) , lower(s.term))
from semesters s
where s.id = $1 
$$ language sql;



-- -- Q7: percentage of international students, S1 and S2, starting from 2005

create or replace function
  prepFunc(integer) returns text
as
$$
select concat(substring(CAST(s.year as CHAR(4)), 3, 2) , lower(s.term))
from semesters s
where s.id = $1  
$$ language sql;


create or replace view Q7(semester, percent)
as
select prepFunc(i.semester),cast(SUM(i.count) * 1.0 / SUM(ts.count) as numeric(4,2))
from totalIntl i, totalStud ts
where i.semester = ts.semester
group by prepFunc
order by prepFunc;

create or replace view totalIntl(semester,count)
as
select pe.semester ,count(s.stype)
from program_enrolments pe , students s, semesters sem
where s.id = pe.student and s.stype = 'intl' and sem.id = pe.semester and sem.year >= 2005 and sem.term NOT LIKE 'X_'
group by pe.semester ;

create or replace view totalStud(semester, count)
as
select pe.semester, count(pe.semester)
from program_enrolments pe , students s, semesters sem
where s.id = pe.student and sem.id = pe.semester and sem.year >= 2004 and sem.term NOT LIKE 'X_'
group by pe.semester;


-- Q8: subjects with > 25 course offerings and no staff recorded


create or replace view step1(subject , id, course)
as
SELECT courses.subject ,courses.id, course_staff.course 
FROM courses
FULL OUTER JOIN course_staff
ON courses.id=course_staff.course
ORDER BY courses.id;


create or replace view step2(subject,total)
as
select s.subject , count(s.subject)
from step1 s
where s.course IS NULL
group by s.subject
order by s.subject;

create or replace view step3(subject,total)
as
select s.subject , count(s.subject)
from step1 s
where s.course IS NOT NULL
group by s.subject
order by s.subject;

create or replace view step4(subject, nOfferings)
as
select distinct((subjects.code ||' '|| subjects.name)), s.total
from step2 s , subjects, step3
where s.total > 25 and s.subject = subjects.id and step3.subject = s.subject;

create or replace view step5(subject, nOfferings)
as
select distinct((subjects.code ||' '|| subjects.name)), s.total
from step2 s , subjects, step3
where s.total > 25 and s.subject = subjects.id;


create or replace view Q8(subject, nOfferings)
as
SELECT s5.subject , s5.nOfferings
FROM step5 s5
LEFT JOIN step4 s4
ON s5.subject = s4.subject
WHERE s4.subject IS NULL;


-- -- Q9: find a good research assistant

create or replace view findSubjectss(count)
as
select count(s.code)
from subjects s
where s.code LIKE 'COMP34__';

create or replace view countTotals(unswid, name, total)
as
select p.unswid , p.name , count(p.unswid)
from people p, course_enrolments ce, courses c, subjects s 
where ce.student = p.id and ce.course = c.id 
and c.subject = s.id and s.code LIKE 'COMP34__'
group by p.unswid , p.name;

create or replace view Q9(unswid, name)
as
select ct.unswid , ct.name
from countTotals ct , findSubjectss fs
where ct.total = fs.count;




-- Q10: find all students who had been enrolled in all popular subjects


create or replace view joinTable(sem, year,subject)
as
SELECT Q6(sem.id) , sem.year , c.subject
FROM courses c
FULL OUTER JOIN semesters sem
ON c.semester=sem.id;

create or replace view findSubjects(code, count)
as
select s.code , count(s.code)
from subjects s, joinTable k
where s.code LIKE 'COMP9___' and s.id = k.subject
and k.sem LIKE '__s_' and k.year < 2014 and k.year > 2001
group by s.code
;

create or replace view eachSem(Q6)
as 
select Q6(sem.id)
from semesters sem
where sem.year < 2014 and sem.year > 2001 and Q6(sem.id) LIKE '__s_'
group by Q6(sem.id)
;

create or replace view totalsem(total)
as
select count(*)
from eachsem;

-- fs.code == s.code !!!
create or replace view totalSemesters(code)
as
select fs.code
from findsubjects fs, totalSem ts
where fs.count >= ts.total;

create or replace view countTS(count)
as
select count(*)
from totalSemesters;


create or replace view goodstudents(unswid, name , count)
as
select p.unswid, p.name, count(ce.grade)
from course_enrolments ce, courses c , subjects s,people p, totalSemesters ts
where ts.code = s.code and ce.course = c.id and c.subject = s.id and ce.student = p.id
and (ce.grade = 'HD' or ce.grade = 'DN')
group by p.unswid,p.name
;

create or replace view Q10(unswid, name)
as
select gs.unswid , gs.name
from goodstudents gs , countTs cts
where gs.count = cts.count
;

-- drop view totalSemesters;
-- drop view totalsem;
-- drop view eachsem;
-- drop view findsubjects;







