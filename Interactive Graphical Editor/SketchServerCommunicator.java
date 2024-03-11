import java.io.*;
import java.net.Socket;

/**
 * Handles communication between the server and one client, for SketchServer
 * @author Dickson Alexander and Louis Weisdorf
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Fall 2012; revised Winter 2014 to separate SketchServerCommunicator
 */
public class SketchServerCommunicator extends Thread {
	private Socket sock;					// to talk with client
	private BufferedReader in;				// from client
	private PrintWriter out;				// to client
	private SketchServer server;			// handling communication for

	public SketchServerCommunicator(Socket sock, SketchServer server) {
		this.sock = sock;
		this.server = server;
	}

	/**
	 * Sends a message to the client
	 * @param msg
	 */
	public void send(String msg) {
		out.println(msg);
	}
	
	/**
	 * Keeps listening for and handling (your code) messages from the client
	 */
	public void run() {
		try {
			System.out.println("someone connected");
			
			// Communication channel
			in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
			out = new PrintWriter(sock.getOutputStream(), true);

			// Tell the client the current state of the world
			// TODO: YOUR CODE HERE
			// Loop through shapes in sketch
			for (int shapeID : server.getSketch().getShapeNavigableMap().keySet()) {
				// Sending draw command message
				out.println("DRAW " + server.getSketch().getShapeNavigableMap().get(shapeID));
			}

			// Keep getting and handling messages from the client
			// TODO: YOUR CODE HERE

			String line; // Saving line variable
			while ((line = in.readLine()) != null) { // While input is not null (from client)
				StringParserServer.parseString(line, server); // Passing line and server into parseString method from StringParserServer class
				server.broadcast(line); // Broadcasting line to all clients
			}

			// Clean up -- note that also remove self from server's list, so it doesn't broadcast here
			server.removeCommunicator(this);
			out.close();
			in.close();
			sock.close();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
}
