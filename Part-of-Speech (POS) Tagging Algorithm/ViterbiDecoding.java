import java.io.*;
import java.util.*;

/**
 * Problem Set 5 – Viterbi algorithm for part of speech recognition.
 * @author Dickson Alexander and Louis Weisdorf
 * CS10 Winter 2023
 */

public class ViterbiDecoding {
    Map<String, Map<String, Integer>> transitionsMap; // Saving map of transitions in instance variable
    Map<String, Map<String, Integer>> observationsMap; // Saving map of observations in instance variable
    Map<String, Map<String, Double>> transitionProbabilitiesMap; // Saving map of transition probabilities in instance variable
    Map<String, Map<String, Double>> observationProbabilitiesMap; // Saving map of observation probabilities in instance variable

    /**
     * Creates a map of transitions between parts of speech. The current POS is the outer key, taking an inner map
     * as its value. The inner map contains the next POS and the associated count of the transitions between
     * current and next POS.
     * @param pathName - file name of text file containing POS tags.
     * @throws IOException - in case of errors reading the file, to be handled in main.
     */
    public void transitions(String pathName) throws IOException {
        transitionsMap = new HashMap<>(); // Instantiating transitionsMap
        BufferedReader file = new BufferedReader(new FileReader(pathName)); // Instantiating BufferedReader object to read file

        String start = "#"; // Labeling the "#" as start

        // Reading through file
        while (file.ready()) {
            String line = file.readLine(); // Saving the contents of the current line in variable line
            String[] lineContents = line.split(" "); // Saving a string array of the line contents, as split by whitespace
            String firstCode = lineContents[0]; // Saving first code in the line as firstCode

            // If the start state has not been inserted into the transitionsMap yet
            if (!transitionsMap.containsKey(start)) {
                Map<String, Integer> startMap = new HashMap<>(); // Initializing a new map to identify next states from the start
                startMap.put(firstCode, 1); // Inserting firstCode as a key in startMap with value of 1
                transitionsMap.put(start, startMap); // Inserting the start state as a key in transitionsMap with startMap as value

                // If the start state is already in the transitionsMap
            } else {
                // If we have not seen a transition between start and the firstCode of the current line
                if (!transitionsMap.get(start).containsKey(firstCode)) {
                    transitionsMap.get(start).put(firstCode, 1); // Insert firstCode as key in the startMap with value of 1

                    // If we have already seen a transition between start and the firstCode of the current line
                } else {
                    transitionsMap.get(start).put(firstCode, transitionsMap.get(start).get(firstCode) + 1); // Increment count by 1
                }
            }


            // Looping through string array of line contents
            for (int i = 0; i < lineContents.length - 1; i++) {
                String currentCode = lineContents[i]; // Saving content of current position in currentCode
                String nextCode = lineContents[i + 1]; // Saving content of next position in nextCode
                String lastWord = lineContents[lineContents.length - 1]; // Saving content of last position in lastWord

                // Handling the final punctuation of a line
                if (lastWord.equals(".") || lastWord.equals("!") || lastWord.equals("?")) {
                    Map<String, Integer> codeCount = new HashMap<>(); // Instantiating empty inner map
                    transitionsMap.put(lastWord, codeCount); // Inserting punctuation in transitionsMap with empty inner map
                }

                // If the currentCode is not in the transitionsMap
                if (!transitionsMap.containsKey(currentCode)) {
                    Map<String, Integer> codeCount = new HashMap<>(); // Instantiating inner map
                    codeCount.put(nextCode, 1); // Inserting the nextCode with a count of 1
                    transitionsMap.put(currentCode, codeCount); // Inserting the inner map as a value for the currentCode key

                    // If the transition is in the map
                } else {
                    // If we have not yet recorded a transition between current and nextCode
                    if (!transitionsMap.get(currentCode).containsKey(nextCode)) {
                        transitionsMap.get(currentCode).put(nextCode, 1); // Inserting nextCode as key in the inner map of currentCode with value of 1

                        // If we have already recorded a transition between current and nextCode
                    } else {
                        transitionsMap.get(currentCode).put(nextCode, transitionsMap.get(currentCode).get(nextCode) + 1); // Updating transition count
                    }
                }
            }
        }
        file.close();
    }

    /**
     * Creates a map of observations, tracking parts of speech and associated words. map of observations. The POS is
     * the outer key, taking an inner map as its value. The inner map contains the associated word and the associated
     * count of the observation between the two.
     * @param textFileName - file name of text file containing sentences.
     * @param codeFileName - file name of text file containing POS tags.
     * @throws IOException - in case of errors reading the file, to be handled in main.
     */
    public void observations(String textFileName, String codeFileName) throws IOException {
        observationsMap = new HashMap<>(); // Instantiating observationsMap

        // Instantiating BufferedReader objects to read text- and codeFile, respectively
        BufferedReader textFile = new BufferedReader(new FileReader(textFileName));
        BufferedReader codeFile = new BufferedReader(new FileReader(codeFileName));

        // Reading through file
        while (textFile.ready()) {
            // Saving the contents of the current line in relevant variables
            String textLine = textFile.readLine().toLowerCase(); // Making the text lower case
            String codeLine = codeFile.readLine();

            // Saving string arrays of the relevant line contents, as split by whitespace
            String[] textLineContents = textLine.split(" ");
            String[] codeLineContents = codeLine.split(" ");

            // Looping through string array of line contents
            for (int i = 0; i < textLineContents.length; i++){
                String currentWord = textLineContents[i]; // Saving content of current position from text file line in currentWord
                String currentCode = codeLineContents[i]; // Saving content of current position from code file line in currentCode

                if (!observationsMap.containsKey(currentCode)) { // If the POS is not in the map
                    Map<String, Integer> codeTextMap = new HashMap<>(); // Instantiating inner map
                    codeTextMap.put(currentWord, 1); // Inserting the currentWord with a count of 1
                    observationsMap.put(currentCode, codeTextMap); // Inserting the inner map as a value for the currentCode key
                }

                else { // If the POS is in the map
                    // If we have not yet recorded a pair between currentCode and currentWord
                    if (!observationsMap.get(currentCode).containsKey(currentWord)) {
                        observationsMap.get(currentCode).put(currentWord, 1); // Inserting currentWord as key in the inner map of currentCode with value of 1

                        // If we have already recorded a pair between currentCode and currentWord
                    } else {
                        observationsMap.get(currentCode).put(currentWord, observationsMap.get(currentCode).get(currentWord) + 1); // Updating currentCode–currentWord pair count
                    }
                }
            }
        }
        textFile.close();
        codeFile.close();
    }

    /**
     * Creates a map of probabilities for transitions in the transitionsMap. The current POS is the outer key,
     * taking an inner map as its value. The inner map contains the next POS and the associated probability of the
     * transitions between current and next POS.
     */
    public void transitionProbabilityCalculator() {
        transitionProbabilitiesMap = new HashMap<>(); // Instantiating map for storing transition probabilities

        // Looping through the keys of the outer map in transitionsMap (i.e. the currentCodes)
        for (String currentCode : transitionsMap.keySet()) {
            double rowTotal = 0; // Saving variable rowTotal to store total amount of transitions from a given currentCode
            Map<String, Double> innerMap = new HashMap<>(); // Instantiating inner map to store transitions from currentCode and associated probability

            // Looping through the transitions from the currentCode (i.e. nextCodes)
            for (String nextCode : transitionsMap.get(currentCode).keySet()) {
                rowTotal += transitionsMap.get(currentCode).get(nextCode); // Adding count of each transition to rowTotal
            }

            // Looping through the transitions from the currentCode (i.e. nextCodes)
            for (String nextCode : transitionsMap.get(currentCode).keySet()) {
                // Inserting log probability of each transition into the inner map
                innerMap.put(nextCode, Math.log(transitionsMap.get(currentCode).get(nextCode) / rowTotal));
                // Inserting currentCode and inner map of nextCode and associated log probability into probabilities map
                transitionProbabilitiesMap.put(currentCode, innerMap);
            }
        }
    }

    /**
     * Creates a map of probabilities for observations in the observationsMap. The POS is the outer key, taking an
     * inner map as its value. The inner map contains the associated word and the associated probability of the
     * observation between the two.
     */
    public void observationProbabilityCalculator() {
        observationProbabilitiesMap = new HashMap<>(); // Instantiating map for storing observation probabilities

        // Looping through the keys of the outer map in observationsMap (i.e. the currentCodes)
        for (String currentCode : observationsMap.keySet()) {
            double rowTotal = 0; // Saving variable rowTotal to store total amount of observations of a given currentCode
            Map<String, Double> innerMap = new HashMap<>(); // Instantiating inner map to store observations of currentCode and associated probability

            // Looping through the observations of the currentCode (i.e. currentWords)
            for (String currentWord : observationsMap.get(currentCode).keySet()) {
                rowTotal += observationsMap.get(currentCode).get(currentWord); // Adding count of each observation to rowTotal
            }

            // Looping through the observations of the currentCode (i.e. currentWords)
            for (String nextCode : observationsMap.get(currentCode).keySet()) {
                // Inserting log probability of each observation into the inner map
                innerMap.put(nextCode, Math.log(observationsMap.get(currentCode).get(nextCode) / rowTotal));
                // Inserting currentCode and inner map of currentWord and associated log probability into probabilities map
                observationProbabilitiesMap.put(currentCode, innerMap);
            }
        }
    }

    /**
     * Analyzes previously unseen text, identifying parts of speech based on training data.
     * @param input - text unseen to the model (either from input or file)
     * @return - string of the parts of speech contained in the text.
     */
    public String viterbiPOSDecoding(String input) {
        double unobservedScore = -100.0;  // Penalty score for unseen observations

        ArrayList<String> bestPath = new ArrayList<>(); // Arraylist to keep track of our trellis
        Set<String> currentStates = new HashSet<>(); // A set of states we are visiting
        Map<String, Double> currentScores = new HashMap<>(); // A map with current states and their scores
        currentStates.add("#"); // Adding the start each time we read a new line
        currentScores.put("#", 0.0); // Initializing a score zero to the start

        ArrayList<Map<String, Map<String, Double>>> backTraceList = new ArrayList<>(); // A list with maps to helps us keep track of our trellis

        String line = input.toLowerCase(); // Converting each word in the line to lower case to avoid ambiguity
        String[] observations = line.split(" "); // Converting the line into a list of strings(words)

        // Looping through each observation(word) in the line
        for (int i = 0; i < observations.length; i++) {
            String currentObservation = observations[i]; // Our currently observed word
            Map<String, Map<String, Double>> backTraceOuterMap = new HashMap<>(); // Instantiating backTraceOuterMap
            backTraceList.add(backTraceOuterMap); // Adding backTraceOuterMap to backTraceList
            Set<String> nextStates = new HashSet<>(); // Instantiating nextStates
            Map<String, Double> nextScores = new HashMap<>(); // Instantiating nextScores

            // Looping through all states in currentStates
            for (String currentState : currentStates) {
                // Looping through each possible transition for currentState
                for (String nextState : transitionsMap.get(currentState).keySet()) {
                    Map<String, Double> backTraceInnerMap = new HashMap<>(); // Instantiating backTraceInnerMap

                    nextStates.add(nextState); // Adding nextState to nextStates

                    double observationScore; // Instantiating observationScore

                    // Check if the nextState has currentObservation as a value in observationsMap
                    if (observationsMap.get(nextState).containsKey(currentObservation)) {
                        observationScore = observationProbabilitiesMap.get(nextState).get(currentObservation); // Get observationScore from observationProbabilitiesMap
                    } else {
                        observationScore = unobservedScore; // Set observationScore to equal unobservedScore
                    }

                    double nextScore = currentScores.get(currentState) + transitionProbabilitiesMap.get(currentState).get(nextState) + observationScore; // Update nextScore

                    // If nextState is not in nextScores or if we found a better score for nextState, update its score
                    if (!nextScores.containsKey(nextState) || nextScore > nextScores.get(nextState)) {
                        nextScores.put(nextState, nextScore); // Updating value of nextState in nextScore
                        backTraceInnerMap.put(currentState, nextScore); // Overriding currentState's value in backTraceInnerMap
                        backTraceOuterMap.put(nextState, backTraceInnerMap); // Updating the backTraceOuterMap

                    }
                }
            }
            currentStates = nextStates; // Updating states to visit
            currentScores = nextScores; // Updating scores of states to visit
        }

        // Back tracing to find the best path (path with the highest probability)
        // Saving last element in backTraceList (last observation) in mapAtLastObservation
        Map<String, Map<String, Double>> mapAtLastObservation = backTraceList.get(backTraceList.size() - 1);
        String highestProbabilityNextState = null; // Creating variable to store highestProbabilityNextState
        String highestProbabilityCurrentState = null; // Creating variable to store highestProbabilityNextState
        double highestProbability = Double.NEGATIVE_INFINITY; // Saving highestProbability to be negative infinity (for initial comparison purposes)

        // Looping through keys in the mapAtLastObservation (next states)
        for (String nextState : mapAtLastObservation.keySet()) {
            // Looping through keys in the inner map (current states)
            for (String currentState : mapAtLastObservation.get(nextState).keySet()) {
                // If the probability is higher than highestProbability
                if (mapAtLastObservation.get(nextState).get(currentState) > highestProbability) {
                    highestProbabilityNextState = nextState; // Save nextState as highestProbabilityNextState
                    highestProbabilityCurrentState = currentState; // Save currentState as highestProbabilityCurrentState
                    highestProbability = mapAtLastObservation.get(nextState).get(currentState); // Update highestProbability
                }
            }
        }

        // Adding the highestProbabilityNextState (final state) to the bestPath list
        bestPath.add(highestProbabilityNextState);

        // Looping backwards over the backTraceList, starting at second-to-last observation
        for (int i = backTraceList.size() - 2; i >= 0; i--) {
            // Saving current map in mapAtCurrentObservation
            Map<String, Map<String, Double>> mapAtCurrentObservation = backTraceList.get(i);

            // Looping through states in the mapAtCurrentObservation
            for (String potentialNextState : mapAtCurrentObservation.keySet()) {
                // If the state is equal to highestProbabilityCurrentState
                if (potentialNextState.equals(highestProbabilityCurrentState)) {
                    // Setting highestProbabilityNextState to the highestProbabilityCurrentState at previous observation
                    highestProbabilityNextState = highestProbabilityCurrentState;

                    // Saving current state associated to updated highestProbabilityNextState
                    for (String newCurrentState : mapAtCurrentObservation.get(potentialNextState).keySet()) {
                        highestProbabilityCurrentState = newCurrentState;
                    }
                    break; // Breaking out of loop to prevent states being overridden
                }
            }

            // Adding the highestProbabilityNextState at the start of the bestPath list
            bestPath.add(0, highestProbabilityNextState);
        }

        // Writing parts of speech into output file
        StringBuilder POSLine = new StringBuilder(); // Creating new StringBuilder object POSLine

        // Looping over parts of speech in bestPath list
        for (String POS : bestPath) {
            POSLine.append(POS + " "); // Adding to POSLine
        }

        // Returning a string representation of the POSLine
        return POSLine.toString();
    }

    /**
     * Using the viterbiPOSDecoding method to analyze text from an input file, printing the parts of speech into
     * an output file.
     * @param inputFileName - file containing text not previously seen by the model.
     * @param outputFileName - file for analyzed parts of speech to be written into.
     * @throws IOException - in case of errors reading the file, to be handled in main.
     */
    public void fileViterbi(String inputFileName, String outputFileName) throws IOException {
        BufferedReader inputFile = new BufferedReader(new FileReader(inputFileName)); // Instantiating BufferedReader object to read file of unseen text
        BufferedWriter outputFile = new BufferedWriter(new FileWriter(outputFileName)); // Instantiating BufferedWriter object to write of POS

        // Checking if there is content to read from file
        while (inputFile.ready()) {
            String line = inputFile.readLine(); // Saving line contents in variable line
            // Calling viterbiPOSDecoding method on the line, inserting the result into the output file
            outputFile.write(viterbiPOSDecoding(line) + "\n");
        }

        // Closing files
        inputFile.close();
        outputFile.close();
    }

    /**
     * Using the viterbiPOSDecoding method to analyze text from console input, printing the parts of speech into
     * the console.
     */
    public void inputViterbi() {
        // Printing instruction message for user to input text
        System.out.println("Please write a sentence to be analyzed with parts of speech: ");

        Scanner text = new Scanner(System.in); // Creating new Scanner object to allow user input
        String textInput = text.nextLine(); // Saving user input in variable textInput

        System.out.println(viterbiPOSDecoding(textInput)); // Calling viterbiPOSDecoding method on textInput, printing parts of speech
    }

    /**
     * Checking for errors in the parts of speech produced by the algorithm.
     * @param verificationFile - provided file containing actual parts of speech.
     * @param resultFile - file containing parts of speech produced by viterbi algorithm.
     * @return - error count.
     * @throws IOException - in case of errors reading the file, to be handled in main.
     */
    public int errorDetection(String verificationFile, String resultFile) throws IOException {
        BufferedReader originalFile = new BufferedReader(new FileReader(verificationFile)); // Instantiating BufferedReader object to read file containing original tags
        BufferedReader producedFile = new BufferedReader(new FileReader(resultFile)); // Instantiating BufferedReader object to read file containing produced tags
        int errors = 0; // Saving errors variable

        // Looping over file
        while (originalFile.ready()) {
            String originalLine = originalFile.readLine(); // Saving line contents of originalFile in originalLine
            String producedLine = producedFile.readLine(); // Saving line contents of producedFile in producedLine
            String [] originalPartsOfSpeech = originalLine.split(" "); // Creating string array from originalLine
            String [] producedPartsOfSpeech = producedLine.split(" "); // Creating string array from producedLine

            // Looping through string array (parts of speech in line)
            for (int i = 0; i < originalPartsOfSpeech.length; i++) {
                // If the produced part of speech does not match the original part of speech
                if (!originalPartsOfSpeech[i].equals(producedPartsOfSpeech[i])) {
                    errors++; // Incrementing the error count
                }
            }
        }

        // Returning the error count
        return errors;
    }


    public static void main(String[] args) {
        ViterbiDecoding tester = new ViterbiDecoding(); // Instantiating object of ViterbiDecoding class, tester

        // Calling methods in try block
        try {
            // Calling method to create transitions map
            tester.transitions("inputs/brown-train-tags.txt");

            // Calling method to create observations map
            tester.observations("inputs/brown-train-sentences.txt", "inputs/brown-train-tags.txt");

            // Calling method to calculate transition probabilities
            tester.transitionProbabilityCalculator();

            // Calling method to calculate observations probabilities
            tester.observationProbabilityCalculator();

            // Calling method to execute Viterbi algorithm on files
            tester.fileViterbi("inputs/brown-test-sentences.txt", "inputs/result.txt");

            // Calling method to execute Viterbi algorithm on console input
            tester.inputViterbi();

            // Printing errors detected in parts of speech analysis
            System.out.println(tester.errorDetection("inputs/brown-test-tags.txt", "inputs/result.txt"));

        } catch (IOException ioException) { // Catching possible IOExceptions from issues reading files
            // Printing error message
            System.out.println("An error occurred reading the files.");
        }
    }
}