import java.util.Scanner;

class Best {
	int move;
	int score;
	
	@Override
	public String toString() {
		return "Best [move=" + move + ", score=" + score + "]";
	}
}

class Util {
	static final int COMPUTER = 1;
	static final int PLAYER = 0;
	
	public static int numOfCharOnBoard(String check, String board[]) {
		int count = 0;
		for (int i = 0; i < 9; i++)
			if (board[i].equals(check)) count++;
		return count;
	}
	
	public static boolean isBoardFull(String board[]) {
		return numOfCharOnBoard(" ", board) == 0;
	}
	
	public static String checkWin(String board[]) {
		if (board[0].equals(board[1]) && board[0].equals(board[2])) return board[0];
		else if (board[3].equals(board[4]) && board[3].equals(board[5])) return board[3];
		else if (board[6].equals(board[7]) && board[6].equals(board[8])) return board[6];
		else if (board[0].equals(board[3]) && board[0].equals(board[6])) return board[0];
		else if (board[1].equals(board[4]) && board[1].equals(board[7])) return board[1];
		else if (board[2].equals(board[5]) && board[2].equals(board[8])) return board[2];
		else if (board[0].equals(board[4]) && board[0].equals(board[8])) return board[0];
		else if (board[2].equals(board[4]) && board[2].equals(board[6])) return board[2];
		else return " ";
	}
	
	public static void printTTTBoard(String board[]) { // TODO: replace with GUI
		for (int i = 0; i < 9; i++) {
			if (board[i] == " ") System.out.print(" " + i + " ");
			else System.out.print(" " + board[i] + " ");
			if (i != 2 && i != 5 && i != 8) System.out.print("|");
			if (i == 2 || i == 5) System.out.print("\n------------\n");
		}
		System.out.println();
	}
	
	/**
	 * Minimax algorithm: go through a game tree, assign -1, 0, and 1 to a loss, draw, or win go up a level, that board's score is the min/max based on the player if it's the computer's turn, it should choose the board with the max value if it's the player's turn, the computer would want it to
	 */
	public static Best chooseMove(String board[], int side) {
		Best curBest = new Best(), reply = new Best();

		// terminal node
		String winnerTest = checkWin(board);
		if (winnerTest.equals("X")) { // if X, the player, has won in this terminal node of the game tree
			curBest.move = ' ';
			curBest.score = -1;
			return curBest;
		} else if (winnerTest.equals("O")) { // if O, the computer, has won in this terminal node of the game tree
			curBest.move = ' ';
			curBest.score = 1;
			return curBest;
		} else if (numOfCharOnBoard(" ", board) == 0) { // if a draw has occurred in this terminal node of the game tree
			curBest.move = ' ';
			curBest.score = 0;
			return curBest;
		}

		// not a terminal node
		if (side == COMPUTER)
			curBest.score = -2; // lowest score, so we can find higher ones
		else
			curBest.score = 2; // highest score, so we can find lower ones

		// recursive part, go down one level
		for (int i = 0; i < 9; i++)
			if (board[i].equals(" ")) { // for each available move
				if (side == COMPUTER) {
					board[i] = "O";
					reply = chooseMove(board, PLAYER);
					board[i] = " ";
				} else {
					board[i] = "X";
					reply = chooseMove(board, COMPUTER);
					board[i] = " ";
				}
				// computer wants higher score for self, lower score for player
				if ((side == COMPUTER && reply.score > curBest.score) ||
						(side == PLAYER && reply.score < curBest.score)) {
					curBest.move = i;
					curBest.score = reply.score;
				}
			}
		return curBest;
	}
}

public class Main {
	static final int COMPUTER = 1;
	static final int PLAYER = 0;

	static String board[] = { " ", " ", " ", " ", " ", " ", " ", " ", " " };	

	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		System.out.println("Welcome to Tic Tac Toe.");

		int side = PLAYER, move = -1;

		while (Util.checkWin(board) == " " && !Util.isBoardFull(board)) {
			if (side == PLAYER) {
				Util.printTTTBoard(board);
				do {
					System.out.print("Your move: ");
					move = s.nextInt();
				} while (move < 0 || move > 8 || !board[move].equals(" ")); // ask for move until it's valid
				board[move] = "X";
				side = COMPUTER;
			} else {
				move = Util.chooseMove(board, COMPUTER).move;
				if (move == 32) break;
				board[move] = "O";
				side = PLAYER;
			}
		}
		
		System.out.println("\nFINAL BOARD\n***********");
		Util.printTTTBoard(board);
		System.out.println("***********\n");
		
		if (Util.checkWin(board).equals("X")) System.out.println("you won!"); // should never happen
		else if (Util.checkWin(board).equals("O")) System.out.println("you lost!");
		else if (Util.checkWin(board).equals(" ")) System.out.println("the game was a draw");
		s.close();
	}
}