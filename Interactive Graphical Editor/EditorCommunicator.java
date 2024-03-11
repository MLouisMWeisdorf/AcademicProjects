import java.awt.*;
import java.io.*;
import java.net.Socket;

/**
 * Handles communication to/from the server for the editor
 *
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Fall 2012
 * @author Chris Bailey-Kellogg; overall structure substantially revised Winter 2014
 * @author Travis Peters, Dartmouth CS 10, Winter 2015; remove EditorCommunicatorStandalone (use echo server for testing)
 */
public class EditorCommunicator extends Thread {
	private PrintWriter out;		// to server
	private BufferedReader in;		// from server
	protected Editor editor;		// handling communication for

	/**
	 * Establishes connection and in/out pair
	 */
	public EditorCommunicator(String serverIP, Editor editor) {
		this.editor = editor;
		System.out.println("connecting to " + serverIP + "...");
		try {
			Socket sock = new Socket(serverIP, 4242);
			out = new PrintWriter(sock.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
			System.out.println("...connected");
		}
		catch (IOException e) {
			System.err.println("couldn't connect");
			System.exit(-1);
		}
	}

	/**
	 * Sends message to the server
	 */
	public void send(String msg) {
		out.println(msg);
	}

	/**
	 * Keeps listening for and handling (your code) messages from the server
	 */
	public void run() {
		try {
			// Handle messages
			// TODO: YOUR CODE HERE

			String line; // Saving line variable
			while ((line = in.readLine()) != null) { // While input is not null (from server)
				StringParserClient.parseString(line, editor); // Passing line and server into parseString method from StringParserClient class
			}

		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			System.out.println("server hung up");
		}
	}

	// Send editor requests to the server
	// TODO: YOUR CODE HERE

	/**
	 * Sends message with request to draw to the sketchServerCommunicator
	 * @param shape - drawn shape.
	 */
	public void requestDraw(Shape shape) {
		System.out.println(shape);
		send("DRAW " + shape); }

	/**
	 * Sends message with request to move object to the sketchServerCommunicator
	 * @param shapeID - ID of shape being moved.
	 * @param dx - distance moved on x dimension.
	 * @param dy - distance moved on y dimension.
	 */
	public void requestMove(int shapeID, int dx, int dy){
		send("MOVE " + shapeID + " " + dx + " " + dy);
	}

	/**
	 * Sends message with request to change object color to the sketchServerCommunicator
	 * @param newColor - new color picked in editor.
	 * @param shapeID - ID of shape being recolored.
	 */
	public void requestRecolor(Color newColor, int shapeID) {
		send("RECOLOR " + shapeID + " " + newColor.getRGB());
	}

	/**
	 * Sends message with request to delete object to the sketchServerCommunicator.
	 * @param shapeID - ID of shape being deleted.
	 */
	public void requestDelete(int shapeID){
		send("DELETE " + shapeID);
	}
}
