public class Source {
    public static void main(String[] args) {
        int[][] a = new int[1000][1000];
        int res = 0;

        long startTime = System.nanoTime();

        for (int i = 999; i > 0; --i) {
            for (int j = 999; j > 0; --j) {
                a[j][i] = a[j - 1][i - 1] + 1;
            }
        }

        for (int i = 0; i < 1000; ++i) {
            for (int j = 0; j < 1000; ++j) {
                a[j][i]--;
            }
        }

        long elapsedTime = System.nanoTime() - startTime;

        System.out.println("Total execution time: " + elapsedTime);

    }
}