import java.util.ArrayList;
import java.util.List;

public class City {
	// Holds all the edges
	private List<CityLink> allLinks;
	// Name of the city
	private String currCity;
	// Time needed to wait at each station
	private int transferTime;

	public City(String currCity, int transferTime) {
		this.currCity = currCity;
		this.transferTime = transferTime;
		this.allLinks = new ArrayList<CityLink>();
	}

	public String getCurrCity() {
		return currCity;
	}

	public int getTransferTime() {
		return transferTime;
	}


	public List<CityLink> getAllLinks() {
		return allLinks;
	}



	/**
	 * Loops through all links(edges) to find the specific link for the from-to
	 * city pair.
	 * 
	 * @param fromCity - City traveling from.
	 * @param toCity - City traveling to.
	 * @return - Link that connects to and from city.
	 */
	public CityLink findLink(String fromCity, String toCity) {
		for (CityLink cl : getAllLinks()) {
			if (cl.getFromCity().equals(fromCity) && cl.getToCity().equals(toCity)) {
				return cl;
			}
		}
		return null;
	}

}
