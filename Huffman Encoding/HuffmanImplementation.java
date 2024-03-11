/**
 * @authors Dickson Alexander and Louis Weisdorf
 * CS10 Winter 2023
 * Problem Set 3 – Implementation of Huffman code.
 */

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.PriorityQueue;

public class HuffmanImplementation implements Huffman {
    public Map<Character, Long> countFrequencies(String pathName) throws IOException {
        // Instantiating map holding characters and their frequencies
        Map<Character, Long> characterFrequencies = new HashMap<>();

        // Instantiating BufferedReader object to read file
        BufferedReader file = new BufferedReader(new FileReader(pathName));

        // If the file is empty
        if (!file.ready()) {
            System.out.println("The file you are reading is empty. The frequencies of characters cannot be counted."); // Printing error message
        }

        // Iterating over contents of file
        while (file.ready()) { // While the file is not empty
            Character currentChar = (char) file.read(); // Saving character currently being reviewed in currentChar
            if (characterFrequencies.containsKey(currentChar)) { // If the character is already in the map
                characterFrequencies.put(currentChar, characterFrequencies.get(currentChar) + 1); // Incrementing frequency by 1
            } else { // If the character is not yet in the map
                characterFrequencies.put(currentChar, 1L); // Creating new key containing character and setting frequency to 1
            }
        }

        // Closing the file
        file.close();

        // Returning the map of characters and their frequencies
        return characterFrequencies;
    }

    public BinaryTree<CodeTreeElement> makeCodeTree(Map<Character, Long> frequencies) {
        // Inserting a try/catch block to handle exceptions related to empty files
        try {
            // Adding initial character trees to the priority queue
            PriorityQueue<BinaryTree<CodeTreeElement>> treePriorityQueue = new PriorityQueue<>(new TreeComparator()); // Instantiating priority queue

            for (Map.Entry<Character, Long> character : frequencies.entrySet()) { // Looping over map
                BinaryTree<CodeTreeElement> initialTree = new BinaryTree<>
                        (new CodeTreeElement(character.getValue(), character.getKey())); // Creating a BinaryTree holding object of type CodeTreeElement
                treePriorityQueue.add(initialTree); // Adding the BinaryTree to the priority queue
            }

            // Creating codeTree from initial character trees
            while (treePriorityQueue.size() > 1) { // Loop runs while there is more than one tree in the priority queue
                // Saving the two characters with the lowest frequencies in t1 and t2, respectively
                BinaryTree<CodeTreeElement> t1 = treePriorityQueue.remove(), t2 = treePriorityQueue.remove();
                Long subTreeSum = t1.getData().getFrequency() + t2.getData().getFrequency(); // Calculating sum of frequencies for t1 and t2
                // Creating a tree that holds t1 and t2 as children, has their sum frequencies as data
                BinaryTree<CodeTreeElement> t = new BinaryTree<>(new CodeTreeElement(subTreeSum, null), t1, t2);
                treePriorityQueue.add(t); // Adding the tree to the priority queue
            }

            // Returning the final code tree left in the priority queue
            return treePriorityQueue.remove();

        } catch (NoSuchElementException noSuchElementException) { // Catching exception for when the priority queue is empty (due to empty file)
            System.out.println("The file you are trying to read is empty, and the priority queue is thus empty. No code tree can be constructed."); // Printing error message
            return null; // Returning null
        }
    }

    public Map<Character, String> computeCodes(BinaryTree<CodeTreeElement> codeTree) {
        // Inserting a try/catch block to handle exceptions related to empty files
        try {
            String codeString = ""; // Creating empty string for character codes to be added to
            Map<Character, String> codeMap = new HashMap<>(); // Instantiating a map to contain characters and their codes

            // Handling boundary case when the file contains a single character (with one or several occurrences)
            if (codeTree.size() == 1) {
                codeMap.put(codeTree.getData().getChar(), "0"); // Assigning a code of 0 to the character
            } else { // If the file contains more than a single character
                computeCodesHelper(codeMap, codeTree, codeString); // Calling helper method for computing code
            }

            return codeMap; // Returning map

        } catch (NullPointerException nullPointerException) { // Catching exception for when the tree is empty (due to empty file)
            System.out.println("The file you are trying to read is empty, and the codeTree is thus empty. No codes can be computed."); // Printing error message
            return null; // Returning null
        }
    }

    /**
     * Recursive helper method for computeCodes. Helper method traverses the tree and adds the character and related character
     * code to the map in the computeCodes method.
     * @param codeMap - map of characters and codes from computeCodes method.
     * @param codeTree - code tree from computeCodes method.
     * @param codeString - empty string from computeCodes method.
     */
    public void computeCodesHelper(Map<Character, String> codeMap, BinaryTree<CodeTreeElement> codeTree, String codeString) {
        if (codeTree.isLeaf()) { // If the current node in the tree is a leaf
            codeMap.put(codeTree.getData().getChar(), codeString); // Inserting the character at that leaf in the map with its code
            return;
        }

        if (codeTree.hasLeft()) { // If the node has a left child
            computeCodesHelper(codeMap, codeTree.getLeft(), codeString + "0"); // Recursively calling method for child, adding "0" to code string
        }

        if (codeTree.hasRight()) { // If the node has a right child
            computeCodesHelper(codeMap, codeTree.getRight(), codeString + "1"); // Recursively calling method for child, adding "1" to code string
        }
    }

    public void compressFile(Map<Character, String> codeMap, String pathName, String compressedPathName) throws IOException {
        BufferedReader textInput = new BufferedReader(new FileReader(pathName)); // Instantiating BufferedReader object to read in file
        BufferedBitWriter bitOutput = new BufferedBitWriter((compressedPathName)); // Instantiating BufferedBitWriter object to write in compressed file

        // Looping through the file
        while (textInput.ready()) { // While there is still content to read in the file
            Character currentCharacter = (char) textInput.read(); // Saving character currently being read in currentCharacter, cast as char
            String currentCharacterCode = codeMap.get(currentCharacter); // Retrieving code for the currentCharacter from map

            // Looping through the code (string) for the currentCharacter
            for (int i = 0; i < currentCharacterCode.length(); i++) {
                if (currentCharacterCode.charAt(i) == '0') { // If the character is a 0
                    bitOutput.writeBit(false); // Writing in compressed file as false boolean
                }
                else if (currentCharacterCode.charAt(i) == '1') { // If the character is a 1
                    bitOutput.writeBit(true); // Writing in compressed file as true boolean
                }
            }
        }

        // Closing the files
        textInput.close();
        bitOutput.close();
    }

    public void decompressFile(String compressedPathName, String decompressedPathName, BinaryTree<CodeTreeElement> codeTree) throws IOException {
        BufferedBitReader bitInput = new BufferedBitReader(compressedPathName); // Instantiating BufferedBitReader object to read compressed file
        BufferedWriter textOutput = new BufferedWriter(new FileWriter(decompressedPathName)); // Instantiating BufferedWriter object to write decompressed file

        // Handling boundary case when the file contains a single character (with one or several occurrences)
        if (codeTree.size() == 1) { // If there is only one character in the tree
            for (int i = 0; i < codeTree.getData().getFrequency(); i++) { // Looping over character occurrence(s) according to its frequency
                bitInput.readBit(); // Reading the bit
                textOutput.write(codeTree.getData().getChar()); // Writing the character in the decompressed file
            }
        } else { // If the file contains more than a single character
            // Looping over the compressed file
            while (bitInput.hasNext()) { // // While there is still content to read in the file
                Character currentCharacter = decompressFileHelper(bitInput, codeTree); // Calling helper method, saving in currentCharacter
                if (currentCharacter == null) { // If the currentCharacter is null
                    break; // Breaking out of loop
                }
                textOutput.write(currentCharacter); // Writing currentCharacter in decompressed file
            }
        }

        // Closing files
        bitInput.close();
        textOutput.close();
    }

    /**
     * Recursive helper method for decompressFile. Helper method traverses the tree, returning the decompressed character
     * to the decompressFile method once it reaches a leaf.
     * @param bitInput - decompressed file instantiated in decompressFile method.
     * @param codeTree - code tree of character and frequencies.
     * @return - decompressed character to the decompressFile method.
     * @throws IOException - potential IOExceptions will be handled in the main.
     */
    public Character decompressFileHelper(BufferedBitReader bitInput, BinaryTree<CodeTreeElement> codeTree) throws IOException {
        if (codeTree.isLeaf()) { // If the current node is a leaf
            return codeTree.getData().getChar(); // Return the decompressed character to the decompressFile method
        }

        if (!bitInput.hasNext()) { // If there is not a next bit to read in the compressed file
            return null; // Returning null, preventing infinite loop
        }

        boolean bit = bitInput.readBit(); // Reading the current bit in the compressed file, saving in variable bit
        if (!bit) { // If the bit is false (0)
            return decompressFileHelper(bitInput, codeTree.getLeft()); // Recursively calling method on left child
        } else { // If the bit is true (1)
            return decompressFileHelper(bitInput, codeTree.getRight()); // Recursively calling method on right child
        }
    }

    public static void main(String[] args) throws Exception {
        HuffmanImplementation testImplementation = new HuffmanImplementation(); // Creating instance of class

        // Calling the countFrequencies method in try/catch block
        try {
            testImplementation.countFrequencies("inputs/Tester.txt");
        } catch (FileNotFoundException fileNotFoundException) { // In case the file does not exist
            System.out.println("The file you are trying to read does not exist. Please verify that the file name is correct.");
        }

        // Calling the makeCodeTree method
        testImplementation.makeCodeTree(testImplementation.countFrequencies("inputs/USConstitution.txt"));

        // Calling the computeCodes method
        testImplementation.computeCodes(testImplementation.makeCodeTree(testImplementation.countFrequencies("inputs/USConstitution.txt")));

        // Calling the compressFile method
        testImplementation.compressFile(testImplementation.computeCodes(testImplementation.makeCodeTree(testImplementation.countFrequencies("inputs/USConstitution.txt"))), "inputs/USConstitution.txt", "inputs/USConstitution_compressed.txt");

        // Calling the decompressFile method in try/catch block
        try {
            testImplementation.decompressFile("inputs/USConstitution_compressed.txt", "inputs/USConstitution_decompressed.txt", testImplementation.makeCodeTree(testImplementation.countFrequencies("inputs/USConstitution.txt")));
        } catch (NullPointerException nullPointerException) { // In case the file is empty
            System.out.println("The file you are trying to read is empty. There is no text to decompress.");
        }
    }
}