import java.io.Serializable;
import java.util.*;
import java.awt.*;

/**
 * Class for sketch.
 * @author Dickson Alexander and Louis Weisdorf
 * CS10 Winter 2023
 */

public class Sketch implements Serializable {
    private int id = 1; // Initializing id number at 1.
    private NavigableMap<Integer, Shape> shapeNavigableMap; // Initializing a new navigable map that enables accessing things in ascending and descending order

    /**
     * Constructor for Sketch object.
     */
    public Sketch(){
        shapeNavigableMap = Collections.synchronizedNavigableMap(new TreeMap<Integer, Shape>()); // Making the shapes map synchronized because we will want only one client accessing it at a time
    }

    /**
     * Getter for the navigable map containing IDs and related shapes.
     * @return - navigable map containing IDs and related shapes
     */
    public synchronized NavigableMap<Integer, Shape> getShapeNavigableMap(){ return shapeNavigableMap;} // A getter for shapeNavigableMap

    /**
     * Method to add shape to the map of shapes.
     * @param shape - shape to be added.
     */
    public synchronized void addShape(Shape shape){ // Method to add a new shape to Sketch
        shapeNavigableMap.put(id, shape); // Adding shape to map
        id++; // Incrementing id value
    }

    /**
     * Method remove shape from the map of shapes.
     * @param id - ID of shape to be removed.
     */
    public synchronized void removeShape(int id){ // Method to remove shape from map given id
        shapeNavigableMap.remove(id); // Remove shape from map given id
    }

    /**
     * Method for retrievin the ID of a shape at a certain location.
     * @param x - x-coordinate of location.
     * @param y - y-coordinate of location.
     * @return - ID of shape at location.
     */
    public synchronized Integer getShapeID(int x, int y){
        // Loop through all keys in the shapeNavigableMap
        for (Integer shapeID : shapeNavigableMap.descendingKeySet()) {
            // Check if point (x,y) is part of any shape in map
            if (shapeNavigableMap.get(shapeID).contains(x, y)) {
                return shapeID;
            }
        }
        return -1; // If not found anything, return -1
    }

    /**
     * Method to draw shapes in the map.
     * @param g - graphics object.
     */
    public synchronized void draw(Graphics g) {
        // Loop over every shape in the Sketch
        for (Integer shapeID : shapeNavigableMap.keySet()) {
            shapeNavigableMap.get(shapeID).draw(g); // Draw every shape in Sketch from first put in map to last
        }
    }
}
