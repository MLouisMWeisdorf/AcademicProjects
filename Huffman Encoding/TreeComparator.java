/**
 * @authors Dickson Alexander and Louis Weisdorf
 * CS10 Winter 2023
 * Problem Set 3 – TreeComparator class for priority queue.
 */

import java.util.Comparator;

public class TreeComparator implements Comparator<BinaryTree<CodeTreeElement>> {

    // Writing the compare method to apply to our priority queue
    public int compare(BinaryTree<CodeTreeElement> tree1, BinaryTree<CodeTreeElement> tree2) {
        // If the frequency of tree2 is greater than tree1
        if (tree1.getData().getFrequency() < tree2.getData().getFrequency()) {
            return -1;
        }
        // If the frequency of the trees are equal
        else if (tree1.getData().getFrequency().equals(tree2.getData().getFrequency())) {
            return 0;
        }
        // If the frequency of tree1 is greater than tree2
        else {
            return 1;
        }
    }
}
