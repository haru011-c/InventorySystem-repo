import java.util.Scanner;

public class PauseClear {
    static void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    static void pauseAndClear(Scanner sc) {
        System.out.print("\n  Press ENTER to continue...");
        sc.nextLine();
        clearScreen();
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (true) {
            System.out.print("1. View Catalog\n2. Exit\nEnter choice: ");
            String choice = sc.nextLine().trim();
            if (choice.equals("2")) break;
            System.out.println("\n(catalog output here)");
            pauseAndClear(sc);
        }
    }
}
