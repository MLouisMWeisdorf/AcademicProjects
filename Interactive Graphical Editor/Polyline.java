import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

/**
 * A multi-segment Shape, with straight lines connecting "joint" points -- (x1,y1) to (x2,y2) to (x3,y3) ...
 * @author Dickson Alexander and Louis Weisdorf
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Spring 2016
 * @author CBK, updated Fall 2016
 */
public class Polyline implements Shape {
	// TODO: YOUR CODE HERE
	ArrayList<Point> pointList; // Creating an ArrayList instance variable
	private Color color; // Creating a color instance variable
	private int x1; // Initializing x1 (to hold x coordinate of initial point)
	private int y1; // Initializing y1 (to hold y coordinate of initial point)

	public Polyline(int x1, int y1, Color color) {
		this.x1 = x1; // Initializing x1
		this.y1 = y1; // Initializing y1
		this.color = color; // Initializing color
		pointList = new ArrayList<>(); // Initializing pointList
	}

	/**
	 * A getter method for the pointList instance variable
	 * @return a list of points
	 */
	public ArrayList<Point> getPointList() { return pointList; }

	/**
	 * Method to move polyline to new location
	 * @param dx distance moved along the x-axis
	 * @param dy distance moved along the y-axis
	 */
	@Override
	public void moveBy(int dx, int dy) {
		for (Point point : pointList) {
			point.setLocation(point.getX() + dx, point.getY() + dy);
		}
	}

	/**
	 * A getter method for the color instance variable
	 * @return color polyLine's color
	 */
	@Override
	public Color getColor() { return color; }

	/**
	 * A setter method for the color instance variable
	 * @param color The polyLine's color
	 */
	@Override
	public void setColor(Color color) { this.color = color; }

	/**
	 * A method to check if pressed point is within the polyLine's coordinates
	 * @param x x-coordinate of pressed point
	 * @param y y-coordinate of pressed point
	 * @return - boolean of whether point of within coordinates.
	 */
	@Override
	public boolean contains(int x, int y) {
		// Looping through each point in the pointList and checking if pressed point lies within any segment of the polyLine
		for (int i = 0; i < pointList.size() - 1; i++) {
			if (Segment.pointToSegmentDistance(x, y, (int) pointList.get(i).getX(), (int) pointList.get(i).getY(),
					(int) pointList.get(i + 1).getX(), (int) pointList.get(i + 1).getY()) <= 3) {
				return true;
			}
		}
		return false;
	}

	/**
	 * A draw method for the polyLine
	 * @param g - graphics object.
	 */
	@Override
	public void draw(Graphics g) {
		// Looping through each point in the pointList and making small segment between each two subsequent points
		for (int i = 0; i < pointList.size() - 1; i++) {
			Segment segment = new Segment((int) pointList.get(i).getX(), (int) pointList.get(i).getY(),
					(int) pointList.get(i + 1).getX(), (int) pointList.get(i + 1).getY(), color);

			segment.draw(g);
		}
	}

	/**
	 * A toString method for the polyLine
	 * @return - string of information about polyline.
	 */
	@Override
	public String toString() {
		String completeString = "freehand " + x1 + " " + y1 + " " + color.getRGB() + " "; // Initializing completeString

		// Looping through all points in the pointList
		for (Point point : pointList) {
			String pointInformation = point.getX() + "," + point.getY() + ";"; // Getting x and Y for each point and adding to pointInformation
			completeString += pointInformation; // Updating completeString
		}

		return completeString;
	}
}
