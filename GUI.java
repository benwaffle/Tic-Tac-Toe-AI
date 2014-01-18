import java.awt.EventQueue;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;


public class GUI {
	private JFrame frame;
	private String[] board = {" "," "," "," "," "," "," "," "," "};

	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					GUI window = new GUI();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public GUI() {
		initialize();
	}


	private void initialize() {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException e1) {
			e1.printStackTrace();
		}
		frame = new JFrame();
		frame.setBounds(100, 100, 300, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new GridLayout(3, 3, 0, 0));
		
		JButton[] buttons = new JButton[9];
		for(int i = 0; i < 9; i++){
			buttons[i] = new JButton("");
			buttons[i].setFont(new Font("Arial", Font.PLAIN, 30));
			buttons[i].addActionListener(new ActionListenerWithID(buttons, i, board));
			frame.getContentPane().add(buttons[i]);
		}
	}
}

class ActionListenerWithID implements ActionListener {
	int id = -1;
	String[] board;
	JButton[] guiboard;
	
	public ActionListenerWithID(JButton[] guiboard, int id, String[] board) {
		this.guiboard = guiboard;
		this.id = id;
		this.board = board;
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if(board[id] == " ")
			set("X");
		else
			JOptionPane.showMessageDialog(null, "Box taken, choose a different box");
	}
	
	private void set(String c){
		board[id] = c;
		guiboard[id].setText("X");
		
		int comp = Util.chooseMove(board, Util.COMPUTER).move;
		if (comp != 32){
			board[comp] = "O";
			guiboard[comp].setText("O");
		}
		
		Util.printTTTBoard(board);
		System.out.println();
		
		if(Util.isBoardFull(board) && Util.checkWin(board) == " "){
			JOptionPane.showMessageDialog(null, "Draw");
			reset();
		} else if(Util.checkWin(board) == "O"){
			JOptionPane.showMessageDialog(null, "Computer won");
			reset();
		}
	}
	private void reset(){
		Arrays.fill(board, " ");
		for(JButton jb : guiboard) jb.setText("");
	}
}
