package com.company;
import java.io.File;
import java.io.FileWriter;
import com.opencsv.CSVWriter;

import java.io.IOException;
import java.util.Random;

public class Main {

    public static void main(String[] args) {
        int noVertices = 64;    //16 32 64 128

        graphFor(noVertices, 1,5, "C:\\Users\\langk\\CodeBlocks\\FPGA-dijkstra\\Dijkstra Graph Generator\\src\\com\\company\\sparse64vertices.csv");
    }

    public static void graphFor(int noVertices, int edgeMin, int edgeMax, String filePath){

        // first create file object for file placed at location
        // specified by filepath
        Random rand = new Random();
        File file = new File(filePath);
        try {
            // create FileWriter object with file as parameter
            FileWriter outputfile = new FileWriter(file);

            // create CSVWriter object filewriter object as parameter
            CSVWriter writer = new CSVWriter(outputfile);

            // adding header to csv
            String[] header = { "from", "to", "weight" };
            writer.writeNext(header);

            // add data to csv
            for (int fromVertex = 0; fromVertex < noVertices; fromVertex++){
                int noEdges = rand.nextInt(edgeMin, edgeMax+1);
                for (int j = 0; j < noEdges; j++){
                    int toVertex = generateRandom(0, noVertices, fromVertex);
                    int weight = rand.nextInt(65535)+1;
                    System.out.println("from: "+fromVertex+", to: "+toVertex+", weight: "+weight);
                    String[] data = {""+fromVertex, ""+toVertex, ""+weight};
                    writer.writeNext(data);

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
