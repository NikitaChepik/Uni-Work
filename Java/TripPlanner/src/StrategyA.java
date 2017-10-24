

public class StrategyA implements Strategy{
	
	/**
	 * Calculates the total remaining travel time and transfer
	 * time of the remaining goal trips in order to predict the minimum amount of time 
	 * that is needed in order to complete the goal.
	 */
	public int travelStrategy(State s,Map worldMap) {
	int h = 0;
	for(City c : worldMap.getAllCities()){
		for(String[] st : s.getNeedToVisit()){
			if(c.getCurrCity().equals(st[0]) ){
				h += c.getTransferTime();
			}
		}
	}
	
	for(City c : worldMap.getAllCities()){
		for(String[] st : s.getNeedToVisit()){
			for(CityLink cl : c.getAllLinks()){
				if(st[0].equals(cl.getFromCity()) && st[1].equals(cl.getToCity()))
					h+=cl.getTravelTime();
			}
		}
	}
	
	return h;
	}
	
	
	
	
}