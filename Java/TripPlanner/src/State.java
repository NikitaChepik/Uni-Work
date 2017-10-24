import java.util.ArrayList;
import java.util.List;

public class State implements Comparable<State>{
	private String fromCity;
	private String currCity;
	private int g;
	private int h;
	private int f;
	private List<String[]> needToVisit;
	private State prevState;
	
	public State(String fromCity,String currCity ,int f , int g ,int h,State prevState){
		this.needToVisit = new ArrayList<String[]>();
		this.currCity = currCity;
		this.fromCity = fromCity;
		this.prevState = prevState;
		this.f = f; // total
		this.g = g; // edge
		this.h = h; // heuristic
	}
	
	public void addNeedToVisit(String from, String to){
		String[] needToVisitArray = new String[] {from,to};
		needToVisit.add(needToVisitArray);
	}
	
	public void removeNeedToVisit( CityLink cl) {
		// Find a city pair to remove from the cities to visit.
		for (String[] st : this.getNeedToVisit()) {
			if (st[0].equals(cl.getFromCity()) && st[1].equals(cl.getToCity())) {
				this.getNeedToVisit().remove(st);
				break;
			}
		}
	}
	
	public boolean checkIfRequiredTrip( CityLink cl) {
		// Find a city pair to remove from the cities to visit.
		for (String[] st : this.getNeedToVisit()) {
			if (st[0].equals(cl.getFromCity()) && st[1].equals(cl.getToCity())) {
				return true;
			}
		}
		return false;
	}
	

	/**
	 * Returns 
	 * @param cl
	 * @return
	 */
	public boolean isConnectingTrip(CityLink cl) {
		
		boolean result = false;
		
		// Find a city pair to remove from the cities to visit.
		for (String[] s : this.getNeedToVisit()) {
			String currFromCity = s[0];
			if (currFromCity.equals(cl.getToCity())) {
				result = true;
			}
		}
		return result;
	}
	
	public int getG() {
		return g;
	}

	public void setG(int g) {
		this.g = g;
	}

	public int getH() {
		return h;
	}

	public void setH(int h) {
		this.h = h;
	}

	public int getF() {
		return f;
	}

	public void setF(int f) {
		this.f = f;
	}

	public String getCurrCity() {
		return currCity;
	}

	public void setCurrCity(String currCity) {
		this.currCity = currCity;
	}

	public List<String[]> getNeedToVisit() {
		return needToVisit;
	}

	public void setNeedToVisit(List<String[]> needToVisit) {
		this.needToVisit = needToVisit;
	}



	@Override
	public int compareTo(State s) {
		return this.f - s.getF();
	}

	public String getFromCity() {
		return fromCity;
	}

	public void setFromCity(String fromCity) {
		this.fromCity = fromCity;
	}

	public State getPrevState() {
		return prevState;
	}

	public void setPrevState(State prevState) {
		this.prevState = prevState;
	}



}