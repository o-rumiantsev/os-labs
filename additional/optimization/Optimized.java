public class Optimized {
    public static void main(String[] args) {
        int[][] a = new int[10000][1000];
        int res = 0;

        for (int i = 9999; i > 0; --i) {
            for (int j = 999; j > 0; --j) {
                a[i][j] = a[i - 1][j - 1] + 1;
            }
        }

        for (int i = 0; i < 10000; ++i) {
            for (int j = 0; j < 1000; ++j) {
                a[i][j]--;
            }
        }

    }
}