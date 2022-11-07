package com.company;
import java.io.File;
import java.io.FileWriter;
import com.opencsv.CSVWriter;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;

public class Main {

    public static void main(String[] args) {
        int noVertices = 64;    //16 32 64 128

        makeGraph(noVertices, 1,4, true, "s-d-64-v.csv");       //sparse directed
        makeGraph(noVertices, 1, 4, false, "s-ud-64-v.csv");    //sparse undirected
        makeGraph(noVertices, 20,30, true, "d-d-64-v.csv");     //dense directed
        makeGraph(noVertices, 20, 30, false,"d-ud-64-v.csv");   //dense undirected
    }
    public static void makeGraph(int noVertices, int edgeMin, int edgeMax, Boolean directed, String filePath){
        // first create file object for file placed at location
        // specified by filepath

        Random rand = new Random();
        File file = new File(filePath);
        ArrayList<ArrayList<Integer>> edges = new ArrayList<>();
        try {
            // create FileWriter object with file as parameter
            FileWriter outputfile = new FileWriter(file);

            // create CSVWriter object filewriter object as parameter
            CSVWriter writer = new CSVWriter(outputfile);
            // add data to csv
            for (int fromVertex = 0; fromVertex < noVertices; fromVertex++){
                int noEdges = rand.nextInt(edgeMin, edgeMax+1);
                for (int j = 0; j < noEdges; j++) {
                    int toVertex = generateRandom(0, noVertices, fromVertex);
                    int weight = rand.nextInt(65535)+1;

                    addEdge(writer, edges, fromVertex, toVertex, weight);
                    if (!directed) addEdge(writer, edges, toVertex, fromVertex, weight);
                }
                if (directed){
                    int guaranteedFrom = generateRandom(0,noVertices, fromVertex);
                    int weight = rand.nextInt(65535)+1;
                    addEdge(writer, edges, guaranteedFrom, fromVertex, weight);
                }
            }
            // closing writer connection
            writer.close();
        }
        catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public static void addEdge(CSVWriter w, ArrayList<ArrayList<Integer>> edges, int from, int to, int weight){
        ArrayList<Integer> list = new ArrayList<>();
        list.add(from); list.add(to);
        if (!edges.contains(list)) {
            System.out.println("from: " + from + ", to: " + to + ", weight: " + weight);
            String[] data = {String.valueOf(from), String.valueOf(to), String.valueOf(weight)};
            w.writeNext(data);
            edges.add(list);
        }
    }

    public static int generateRandom(int start, int end, int exclude) {
        Random rand = new Random();
        int range = end - start;

        int random = rand.nextInt(range);
        while(exclude == random) {
            random = rand.nextInt(range);
        }

        return random;
    }
}
