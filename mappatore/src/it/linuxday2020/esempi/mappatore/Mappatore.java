package it.linuxday2020.esempi.mappatore;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


public class Mappatore {
	char[][] map;
	int r = 0, c = 0;
	final char visited = 'X';

	Mappatore(String mapDir) {
		int start, end;
		String chosen = chooseMap(mapDir);

		File mapFile = new File(chosen);

		readMap(mapFile);
		printMap();
		start = findCharInLine(0, 'S');
		end = findCharInLine(map.length - 1, 'E');
		System.out.printf("Da (%d,%d) a (%d,%d)%n", 0, start, map.length - 1, end);

		int currentR = 0, currentC = start;
		int destinationR = map.length - 1, destinationC = end;
		while(currentR != destinationR || currentC != destinationC) {
			int nextR, nextC;

			if(currentR < destinationR) {
				nextR = currentR + 1;
			} else {
				nextR = currentR;
			}

			if(currentC == destinationC) {
				nextC = currentC;
			} else {
				int distanceC = destinationC - currentC;
				distanceC /= Math.abs(distanceC);

				nextC = currentC + distanceC;
				if(isForbidden(nextR, nextC)) {
					if(distanceC == 0) {
						nextC = currentC - 1;
						if(isForbidden(nextR, nextC)) {
							nextC = currentC + 1;
							// R,0
							// R,-1
							// R,+1
						}
					} else {
						nextC = currentC;
						if(isForbidden(nextR, nextC)) {
							nextC = currentC - distanceC;
							// R,+-1
							// R,0
							// R,-+1
						}
					}
				}
			}

			if(isForbidden(nextR, nextC)) {
				if(nextR > currentR) {
					nextR = currentR;
					nextC = currentC + 1;
					if(isForbidden(nextR, nextC)) {
						nextC = currentC - 1;
						if(isForbidden(nextR, nextC)) {
							break;
						}
					}
				} else {
					break;
				}
			}

			map[nextR][nextC] = visited;
			currentR = nextR;
			currentC = nextC;
		}

		boolean failed = false;
		if(currentR != destinationR && currentC != destinationC) {
			failed = true;
			map[currentR][currentC] = '?';
		} else {
			map[currentR][currentC] = '!';
		}

		printMap();

		if(failed) {
			System.out.printf("Bloccato in (%d,%d)!%n", currentR, currentC);
		}
	}

	private boolean isForbidden(int r, int c) {
		return r < 0 || r >= this.r || c < 0 || c >= this.c ||  map[r][c] == '#' || map[r][c] == visited;
	}

	private int findCharInLine(int line, char toFind) {
		for(int c = 0; c < map[line].length; c++) {
			if(map[line][c] == toFind) {
				return c;
			}
		}
		return 0;
	}

	private void printMap() {
		for(char[] chars : map) {
			for(char aChar : chars) {
				System.out.print(aChar);
			}
			System.out.println();
		}
	}

	private void readMap(File mapFile) {
		countLines(mapFile);

		try(Scanner fileScanner = new Scanner(mapFile)) {
			map = new char[r][c];
			int rr = 0;
			while(fileScanner.hasNextLine()) {
				String line = fileScanner.nextLine();
				for(int cc = 0; cc < line.length(); cc++) {
					char cell = line.charAt(cc);
					map[rr][cc] = cell;
				}
				rr++;
			}
			c = map[0].length;
			System.out.printf("Righe: %d, colonne: %d%n", r, c);
		} catch(FileNotFoundException e) {
			System.out.println("Il file " + mapFile + " non esiste");
			System.exit(1);
		}
	}

	private void countLines(File mapFile) {
		// Count lines
		try(Scanner fileScanner = new Scanner(mapFile)) {
			while(fileScanner.hasNextLine()) {
				r++;
				if(c <= 0) {
					c = fileScanner.nextLine().length();
				} else {
					fileScanner.nextLine();
				}
			}
		} catch(FileNotFoundException e) {
			System.out.println("Il file " + mapFile + " non esiste");
			System.exit(1);
		}
	}

	private String chooseMap(String mapDir) {
		String[] mapFiles;
		File f = new File(mapDir);
		mapFiles = f.list();
		if(mapFiles == null) {
			System.out.println("Non ci sono mappe in " + mapDir);
			System.exit(1);
		}

		System.out.println("Mappe disponibili:");
		int i = 1;
		for(String pathname : mapFiles) {
			System.out.println(i + ") " + pathname);
			i++;
		}

		Scanner s = new Scanner(System.in);
		int map = -1;
		while(map < 1 || map > mapFiles.length) {
			System.out.print("Mappa scelta: ");
			map = Integer.valueOf(s.nextLine(), 10);
		}

		return mapDir + "/" + mapFiles[map - 1];
	}
}

