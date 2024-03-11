import java.awt.*;

/**
 * @author Dickson Alexander and Louis Weisdorf
 * Class to help with handling of messages communicated to the server side.
 */

public class StringParserServer {
    /**
     * Static method to help parse the messages between client and server then perform required operation.
     * @param input A message communicated between client and server
     * @param server An object of type SketchServer
     */
    public static void parseString(String input, SketchServer server) {

        String[] inputContents = input.split(" "); // Splitting the input
        String commandType = inputContents[0]; // Labelling first word from input

        // Checking if mode is DRAW
        if (commandType.equals("DRAW")) {
            String shapeType = inputContents[1]; // Getting the shape type


            // Checking if shape type is freehand
            if (shapeType.equals("freehand")) {
                // Getting coordinates (x1,y1) and RGB value from input
                int x1 = Integer.parseInt(inputContents[2]);
                int y1 = Integer.parseInt(inputContents[3]);
                int colorValue = Integer.parseInt(inputContents[4]);
                // The latter part of the string contains the points in the polyline
                String pointCoordinates = inputContents[5];

                Polyline polyline = new Polyline(x1, y1, new Color(colorValue)); // Creating a new polyLine object

                String[] points = pointCoordinates.split(";"); // Splitting pointCoordinates

                // Looping through all points in the points list
                for (String pointString : points) {
                    String[] coordinates = pointString.split(","); // Splitting each element in points
                    String x = coordinates[0]; // Getting x-coordinate
                    String y = coordinates[1]; // Getting y-coordinate
                    int intX = (int) Double.parseDouble(x); // Casting string to double then integer
                    int intY = (int) Double.parseDouble(y); // Casting string to double then integer
                    Point point = new Point(intX, intY); // Creating new point
                    polyline.getPointList().add(point); // Adding new point to the Polyline object's pointList
                }

                server.getSketch().addShape(polyline); // Updating the state of the world (adding shape to sketch)
            }

            else {
                // Assigning x1, x2, y1, y2, and color variables values from input
                int x1 = Integer.parseInt(inputContents[2]);
                int y1 = Integer.parseInt(inputContents[3]);
                int x2 = Integer.parseInt(inputContents[4]);
                int y2 = Integer.parseInt(inputContents[5]);
                int colorValue = Integer.parseInt(inputContents[6]);

                // Checking if  shape type is ellipse
                if (shapeType.equals("ellipse")) {
                    Ellipse ellipse = new Ellipse(x1, y1, x2, y2, new Color(colorValue)); // Creating a new ellipse object
                    server.getSketch().addShape(ellipse); // Updating the state of the world
                }

                // Checking if  shape type is rectangle
                else if (shapeType.equals("rectangle")) {
                    Rectangle rectangle = new Rectangle(x1, y1, x2, y2, new Color(colorValue)); // Creating a new rectangle object
                    server.getSketch().addShape(rectangle); // Updating the state of the world
                }

                // Checking if  shape type is segment
                else if (shapeType.equals("segment")) {
                    Segment segment = new Segment(x1, y1, x2, y2, new Color(colorValue)); // Creating a new segment object
                    server.getSketch().addShape(segment); // Updating the state of the world
                }
            }
        }

        // Checking if mode is MOVE
        else if (commandType.equals("MOVE")) {
            int shapeID = Integer.parseInt(inputContents[1]); // Getting the shape ID from input
            int dx = Integer.parseInt(inputContents[2]); // Getting dx from input
            int dy = Integer.parseInt(inputContents[3]); // Getting dy from input

            Shape shape = server.getSketch().getShapeNavigableMap().get(shapeID); // Getting the shape from the sketch
            shape.moveBy(dx, dy); // Updating a shape's position on the editor canvas

        }

        // Checking if mode is RECOLOR
        else if (commandType.equals("RECOLOR")) {
            int shapeID = Integer.parseInt(inputContents[1]); // Getting the shape ID from input
            int colorValue = Integer.parseInt(inputContents[2]); // Getting RGB value from input
            Color newColor = new Color(colorValue); // Creating color object with RGB value

            server.getSketch().getShapeNavigableMap().get(shapeID).setColor(newColor); // Updating a shape's color on the editor canvas
        }

        // Checking if mode is DELETE
        else if (commandType.equals("DELETE")) {
            int shapeID = Integer.parseInt(inputContents[1]); // Getting the shape ID from input
            server.getSketch().removeShape(shapeID); // Removing the shape from the state of the world
        }
    }
}


