import std.stdio;
import std.string;
import std.algorithm;
import std.conv;
import Path;
import PathList;


void main() {
    int[][] arc_info = [];
    int start_point = 0;
    auto fin = File("arc_info_10.txt");

    // Arc情報の読み込み
    foreach(input; fin.byLine) {
        int[] line;
        foreach(num; input.split(" ")) {
            line ~= num.to!int;
        }
        arc_info ~= line;
    }

    PathList path_list = new PathList(start_point);

    // 全てのpathの内容とコストを表示
    path_list.setOptimalPath("AE", arc_info);
    writeln("Optimal Path: ", path_list.getOptimalPath.get, "(", path_list.getOptimalPath.getCost, ")");
}
