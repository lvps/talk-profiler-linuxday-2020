package it.linuxday2020.esempi.mappatore;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
	    if(args.length < 1) {
	        System.out.println("Forniscimi il percorso delle mappe");
	        System.exit(1);
        }

	    new Mappatore(args[0]);
    }
}
