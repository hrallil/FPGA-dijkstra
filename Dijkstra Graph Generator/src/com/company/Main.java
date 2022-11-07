package com.company;
import java.io.File;
import java.io.FileWriter;
import com.opencsv.CSVWriter;

import java.io.IOException;
import java.util.Random;

public class Main {

    public static void main(String[] args) {
        int noVertices = 64;    //16 32 64 128

        makeDirectedGraph(noVertices, 0,4, "s-d-64-v.csv");
        makeUndirectedGraph(noVertices, 0, 4, "s-ud-64-v.csv");
        makeDirectedGraph(noVertices, 20,30, "d-d-64-v.csv");
        makeUndirectedGraph(noVertices, 20, 30, "d-ud-64-v.csv");
    }
    public static void makeUndirectedGraph(int noVertices, int edgeMin, int edgeMax, String filePath){
        // first create file object for file placed at location
        // specified by filepath
        Random rand = new Random();
        File file = new File(filePath);
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
                    addEdge(writer, fromVertex, toVertex, weight);
                    addEdge(writer, toVertex, fromVertex, weight);
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
    public static void makeDirectedGraph(int noVertices, int edgeMin, int edgeMax, String filePath){
        // first create file object for file placed at location
        // specified by filepath
        Random rand = new Random();
        File file = new File(filePath);
        try {
            // create FileWriter object with file as parameter
            FileWriter outputfile = new FileWriter(file);

            // create CSVWriter object filewriter object as parameter
            CSVWriter writer = new CSVWriter(outputfile);
            // add data to csv
            for (int fromVertex = 0; fromVertex < noVertices; fromVertex++){
                int noEdges = rand.nextInt(edgeMin, edgeMax+1);
                for (int j = 0; j < noEdges; j++){
                    int toVertex = generateRandom(0, noVertices, fromVertex);
                    int weight = rand.nextInt(65535)+1;
                    addEdge(writer, fromVertex,toVertex,weight);
                }
                int guaranteedFrom = generateRandom(0,noVertices, fromVertex);
                int weight = rand.nextInt(65535)+1;
                String[] data = {String.valueOf(guaranteedFrom), String.valueOf(fromVertex), String.valueOf(weight)};
                writer.writeNext(data);
            }
            // closing writer connection
            writer.close();
        }
        catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public static void addEdge(CSVWriter w, int from, int to, int weight){

        System.out.println("from: "+from+", to: "+to+", weight: "+weight);
        String[] data = {String.valueOf(from), String.valueOf(to), String.valueOf(weight)};
        w.writeNext(data);
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
