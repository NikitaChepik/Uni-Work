import java.util.ArrayList;
import java.util.Stack;
import java.util.List;
import java.util.PriorityQueue;

public class Map {
	// List containing all the cities that exist in this graph.
	private List<City> allCities;
	// Total nodes expanded .
	int nodesExpanded = 0;
	private Strategy strat;
	
	public Map() {
		this.allCities = new ArrayList<City>();
		strat = new StrategyA();
	}
	
	
	public List<City> getAllCities() {
		return allCities;
	}

	public void setAllCities(List<City> allCities)  {
		this.allCities = allCities;
	}
	
	/**
	 * Constructs all the edges in the graph by iterating through
	 * all of the fromCity and toCity pairs and adding them forwards
	 * and backwards.
	 * @param fromCity - Name of the city traveling from.
	 * @param toCity - Name of the city traveling to.
	 * @param travelTime - Total time between the from and to city.
	 */
	public void addLinks(String fromCity, String toCity, int travelTime) {
		for (City c : allCities) {
			if (fromCity.equals(c.getCurrCity())) {
				CityLink link = new CityLink(fromCity, toCity, travelTime);
				c.getAllLinks().add(link);
			}
			if (toCity.equals(c.getCurrCity())) {
				CityLink backLink = new CityLink(toCity, fromCity, travelTime);
				c.getAllLinks().add(backLink);
			}
		}
	}
	
	/**
	 * 
	 * @param cityName - Name of the city.
	 * @param transferTime - Time taken to travel between two cities.
	 */
	public void addCity(String cityName, int transferTime) {
		City newCity = new City(cityName, transferTime);
		this.allCities.add(newCity);
	}

	
	/**
	 * Given the string of a city finds 
	 * finds the city with that name and returns
	 * the City object for it.
	 * @param cityName - String name of city
	 * @return - Current city 
	 */
	private City getCity(String cityName) {
		for (City c : allCities) {
			if (cityName.equals(c.getCurrCity())) {
				return c;
			}
		}
		return null;
	}
	
	/**
	 * Taking in a state return all the of 
	 * possible travel states.
	 * @param s - State to be expanded
	 * @return - Child states 
	 */
	public List<State> expandState(State s) {

		// Store the output.
		List<State> childStates = new ArrayList<State>();
		// Store the current city.
		String currCityName = s.getCurrCity();
		City currCity = getCity(currCityName);

		
		for (CityLink cl : currCity.getAllLinks()) {
			// Check if the current fromCity and toCity are a required trip.
			// If so, make a new state which makes this trip and add it to the
			// list of child states.Then remove the trip from the list of trips needed.
			if (s.checkIfRequiredTrip(cl)) {
				State temp = makeState(cl.getFromCity(), cl.getToCity(), s);
				temp.removeNeedToVisit(cl);
				temp.setH(strat.travelStrategy(temp, this));
				temp.setF(temp.getG() + temp.getH());
				childStates.add(temp);
				// Else, check if the current toCity is a fromCity on a required
				// trip. If so, make a new state which makes this trip.
			} else if (s.isConnectingTrip(cl)) {
				State temp = makeState(cl.getFromCity(), cl.getToCity(), s);
				temp.setH(strat.travelStrategy(temp, this));
				temp.setF(temp.getG() + temp.getH());
				childStates.add(temp);
				
			}

		}
		return childStates;

	}
	
	/**
	 * Creates a new state ,copy over the the previous state and 
	 * updates it for the new trip that has been taken.
	 * @param fromCity - Name of city for current city in prevState.
	 * @param toCity - City that is going to be traveled to.
	 * @param prevState - State that we are expanding from.
	 * @return - The new state that is being made.
	 */
	public State makeState(String fromCity, String toCity, State prevState) {

		// Find the link to get the travel time between the two cities
		City c = getCity(fromCity); 
		CityLink cl = c.findLink(fromCity, toCity); 
		// Total g travel time
		int g = prevState.getG() + c.getTransferTime() + cl.getTravelTime();
		State newState = new State(fromCity, toCity, 0, g, 0,prevState);
		newState.setPrevState(prevState);
		// Copy over information from prev State and pop off the link we visited.
		for (String[] s : prevState.getNeedToVisit()) {
			newState.getNeedToVisit().add(s);
		}
		return newState;

	}
	
	/**
	 *  Takes in a state and checks if all the goal trips have been
	 *  completed if expand state
	 * @param initialState - state that has our trip info.
	 */
	public void aStarSearch(State initialState) {
		// Initialize a priority queue to store nodes found in A* search.
		PriorityQueue<State> q = new PriorityQueue<State>();

		// Store number of nodes expanded in the search.
		int nodesExpanded = 0;

		// Store a list of visited states.
		List<State> visitedStates = new ArrayList<State>();

		// Add the initial state to the queue.
		q.add(initialState);

		// Keep track of the current state in A* search.
		State currState;

		// As we use the from city as the transfer counter
		// we need to remove London from the total.
		int initialTransfer = getCity(initialState.getCurrCity()).getTransferTime();

		// Start A* search.
		while (!q.isEmpty()) {

			// Pop the top element off the queue and store as the current state.
			currState = q.poll();
			nodesExpanded++;
			
			// Add the current state to the list of visited nodes.
			visitedStates.add(currState);

			
			// Check if the current state is at the goal and make early exit
			// if it is.
			if (currState.getNeedToVisit().isEmpty()) {
				System.out.println(nodesExpanded + " nodes expanded");
				int totalTravel = currState.getG() - initialTransfer;
				System.out.println("cost = " + totalTravel);
				
				// Create stack to hold all of the states for the ideal solution 
				Stack<State> stateStack = new  Stack<State>();
				State tempState = currState;
				// Push all states to stack.
				while(tempState.getPrevState() != null){
					stateStack.push(tempState);
					tempState = tempState.getPrevState();
				}
				stateStack.push(tempState);
				// Take off the stack and print.
				stateStack.pop();
				while(!stateStack.empty()){
					State temp = stateStack.pop();
					System.out.println("Trip "+temp.getFromCity() +" to "+temp.getCurrCity());
				}
				break;
			}

			
			
			List<State> childStates = expandState(currState);

			// Loop through all the child states if it hasn't been visited 
			//add it to the queue.
			for (State childState : childStates) {
				if (visitedStates.contains(childState)) {
					continue;
				} else {
					q.add(childState);

				}
			
			}

		}

	
	}

}
