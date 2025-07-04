export module UnitTests_Coding_BalanceBoard;


// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

//int FindCharsCount(const string& row, char cToFind)
//{
//    int count = 0;
//    for (char c : row)
//    {
//        if (c == cToFind)
//        {
//            ++count;
//        }
//    }
//
//    return count;
//}
//
//int solution(string& row1, string& row2)
//{
//    const int len = row1.size();
//
//    const int row_1_R = FindCharsCount(row1, 'R');
//    const int row_1_W = FindCharsCount(row1, 'W');
//
//    const int row_2_R = FindCharsCount(row2, 'R');
//    const int row_2_W = FindCharsCount(row2, 'W');
//
//    // If existing R or W count is already greater than half of the board
//    // It's impossible to balance it, not enough question marks 
//    const int halfLen = len / 2;
//    if (row_1_R > halfLen ||
//        row_1_W > halfLen ||
//        row_2_R > halfLen ||
//        row_2_W > halfLen)
//    {
//        return -1;
//    }
//
//    // Calculate how many R and W to balance the board
//    int row_1_R_target = halfLen - row_1_R;
//    int row_1_W_target = halfLen - row_1_W;
//    int row_2_R_target = halfLen - row_2_R;
//    int row_2_W_target = halfLen - row_2_W;
//
//    // Result is the total target question marks to fill the boards to balance it
//    return row_1_R_target + row_1_W_target +
//        row_2_R_target + row_2_W_target;
//}
