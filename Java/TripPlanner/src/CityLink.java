public class CityLink {
	private String fromCity;
	private String toCity;
	private int travelTime;

	public CityLink(String fromCity, String toCity, int travelTime) {
		this.fromCity = fromCity;
		this.toCity = toCity;
		this.travelTime = travelTime;
	}

	public String getFromCity() {
		return fromCity;
	}

	public String getToCity() {
		return toCity;
	}

	public int getTravelTime() {
		return travelTime;
	}

}
