import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 * Runtime analysis of heuristic.
 * 
 * StrategyA is the heuristic that i have decided to implement
 * It runs in o(n^2) the first total transfer time calculation takes
 * n^2 and the travel time calculation is n^2 so the total time is n^2 + n^2  = 2n^2
 * which is just o(n^2)
 * 
 * The heuristic calculates the total time remaining for an ideal solution by adding the remaining
 * trips and the transfer times for those trips. It is admissible because the estimate will 
 * always be equal to or less than the total cost .
 */

public class TripPlanner {
  
	public static void main(String[] args) {
		
		
		Map worldMap = new Map();
		State initialState = new State("London","London", 0, 0, 0,null);

		File f = new File(args[0]);
		Scanner sc = null;
		try {
			sc = new Scanner(f);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		String currLine = "";
		while (sc.hasNextLine()) {
			currLine = sc.nextLine();

			String[] currLineArray = currLine.split(" ");
			String firstWord = currLineArray[0];
			
			// Get info from input file and store it.
			if (firstWord.equals("Transfer")) {
				int transferTime = Integer.parseInt(currLineArray[1]);
				String cityName = currLineArray[2];
				worldMap.addCity(cityName, transferTime);
			}
			if (firstWord.equals("Time")) {
				int travelTime = Integer.parseInt(currLineArray[1]);
				String toCity = currLineArray[2];
				String fromCity = currLineArray[3];
				worldMap.addLinks(fromCity, toCity, travelTime);
			}

			if (firstWord.equals("Trip")) {
				String fromCity = currLineArray[1];
				String toCity = currLineArray[2];
				initialState.addNeedToVisit(fromCity, toCity);

			}
		}
		{
			if (sc != null)
				sc.close();
		}

		// Search with A* given the initial state of London.
		worldMap.aStarSearch(initialState);

	}
	

}
