/*
@author: Avanish Kumar Singh
@email: avanish269@gmail.com
@entry#: 2017MT10728
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Implementation of algorithm to take input from the given text file
 *
 * The input is given in a text file, where the first line
 * is the number of teams in division and then n lines containing
 * team names, wins, losses, remaining games and remaining games amongst themselves.
 * @param filename
 */
vector<string> txtToString(string filename)
{
    vector<string> v;
    fstream file;
    try
    {
        file.open(filename, ios::in);
        if (file.is_open())
        {
            string input;
            while (getline(file, input))
            {
                v.push_back(input);
            }
            file.close();
        }
        else
            throw filename;
    }
    catch (string s)
    {
        cout << __FILE__ << ": error: '" << filename << "' does not exist\n";
    }
    return v;
}

/**
 * Converts given string into the given number by exploiting ASCII code
 * @param numberAsString
 */
int stringToNumber(string s)
{
    int n = 0;
    for (int i = 0; i < s.size(); i++)
    {
        n = 10 * n + (s[i] - 48);
    }
    return n;
}

class divison
{
private:
    int n;
    vector<string> teamNames;
    vector<int> wins, loss, left;
    vector<vector<int>> games;
public:
    divison(vector<string> v)
    {
        int n = stringToNumber(v[0]);
        v.erase(v.begin());
        this->n = n;
        for (int i = 0; i < n; i++)
        {
            int j = 0;
            vector<int> temp;
            this->games.push_back(temp);
            istringstream s(v[i]);
            do {
                string word;
                s >> word;
                switch (j)
                {
                case 0: this->teamNames.push_back(word); break;
                case 1: this->wins.push_back(stringToNumber(word)); break;
                case 2: this->loss.push_back(stringToNumber(word)); break;
                case 3: this->left.push_back(stringToNumber(word)); break;
                default: this->games[i].push_back(stringToNumber(word)); break;
                }
                j++;
            } while (s);
        }
    }

    int getNTeams()
    {
        return this->n;
    }

    vector<string> getTeamNames()
    {
        return this->teamNames;
    }

    vector<int> getWins()
    {
        return this->wins;
    }

    vector<int> getLoss()
    {
        return this->loss;
    }

    vector<int> getLeft()
    {
        return this->left;
    }

    vector<vector<int>> getGames()
    {
        return this->games;
    }
};

class flowNetwork
{
private:
    int n, leftTeam, ngames, nteams;
    vector<string> nodes, teamNames;
    vector<vector<int>> cap, edges;
    vector<vector<bool>> adjMat;
public:
    flowNetwork(divison d, int curTeam)
    {
        int numTeams = d.getNTeams() - 1, numGames = (d.getNTeams() - 1) * (d.getNTeams() - 2) / 2;
        int totalNodes = numTeams + numGames + 2;
        vector<vector<int>> fn(totalNodes, vector<int>(totalNodes, 0));
        vector<vector<int>> e(totalNodes);
        vector<vector<bool>> am(totalNodes, vector<bool>(totalNodes, false));
        vector<string> nod(totalNodes, "");
        nod[0] = "s";
        nod[totalNodes - 1] = "t";
        int k = 1, skip = 0;
        for (int i = 0; i < d.getNTeams(); i++)
        {
            vector<int> temp = d.getGames()[i];
            if (i == curTeam)
                continue;
            for (int j = i + 1; j < d.getNTeams(); j++)
            {
                if (j == curTeam)
                    continue;
                fn[0][k] = temp[j];
                am[0][k] = true;
                e[0].push_back(k);
                nod[k] = to_string(i) + "-" + to_string(j);
                fn[k][0] = 0;
                if (i > curTeam)
                    skip = 1;
                else
                    skip = 0;
                fn[k][numGames + i + 1 - skip] = INT_MAX;
                nod[numGames + i + 1 - skip] = d.getTeamNames()[i];
                am[k][numGames + i + 1 - skip] = true;
                e[k].push_back(numGames + i + 1 - skip);
                if (j > curTeam)
                    skip = 1;
                else
                    skip = 0;
                fn[k][numGames + j + 1 - skip] = INT_MAX;
                nod[numGames + j + 1 - skip] = d.getTeamNames()[j];
                am[k][numGames + j + 1 - skip] = true;
                e[k].push_back(numGames + j + 1 - skip);
                if (i > curTeam)
                    skip = 1;
                else
                    skip = 0;
                fn[numGames + i + 1 - skip][k] = 0;
                if (j > curTeam)
                    skip = 1;
                else
                    skip = 0;
                fn[numGames + j + 1 - skip][k] = 0;
                k++;
            }
            if (i > curTeam)
                skip = 1;
            else
                skip = 0;
            fn[numGames + i + 1 - skip][totalNodes - 1] = d.getWins()[curTeam] + d.getLeft()[curTeam] - d.getWins()[i];
            am[numGames + i + 1 - skip][totalNodes - 1] = true;
            e[numGames + i + 1 - skip].push_back(totalNodes - 1);
        }
        this->n = totalNodes;
        this->ngames = numGames;
        this->nteams = numTeams;
        this->nodes = nod;
        this->teamNames = d.getTeamNames();
        this->cap = fn;
        this->adjMat = am;
        this->edges = e;
        this->leftTeam = curTeam;
    }

    vector<int> bfs(vector<vector<int>> resNet)
    {
        vector<int> path(resNet.size(), INT_MIN);
        vector<bool> visited(resNet.size(), false);
        queue<int> traversal;
        visited[0] = true;
        traversal.push(0);
        while (!traversal.empty())
        {
            int i = traversal.front();
            traversal.pop();
            for (int j = 0; j < this->edges[i].size(); j++)
            {
                if (!visited[edges[i][j]] && resNet[i][edges[i][j]])
                {
                    visited[edges[i][j]] = true;
                    path[edges[i][j]] = i;
                    traversal.push(edges[i][j]);
                    if (visited[this->n - 1])
                        return path;
                }
            }
        }
        return path;
    }

    void maxFlowFordFulkerson(divison d)
    {
        vector<int> path;
        vector<vector<int>> resNet(this->n, vector<int>(this->n, 0));
        int maxFlow = 0, saturation = 0;
        resNet = this->cap;
        for (int j = 0; j < this->n; j++)
            saturation += this->cap[0][j];
        path = this->bfs(resNet);
        while (path[this->n - 1] != INT_MIN)
        {
            int bottleneck = INT_MAX;
            for (int i = this->n - 1; i > 0; i = path[i])
                bottleneck = bottleneck < resNet[path[i]][i] ? bottleneck : resNet[path[i]][i];
            for (int i = this->n - 1; i > 0; i = path[i])
            {
                resNet[path[i]][i] -= bottleneck;
                resNet[i][path[i]] += bottleneck;
                if (find(this->edges[i].begin(), this->edges[i].end(), path[i]) == this->edges[i].end())
                {
                    this->edges[i].push_back(path[i]);
                    sort(this->edges[i].begin(), this->edges[i].end());
                }
                if (resNet[path[i]][i] == 0)
                    this->edges[path[i]].erase(find(this->edges[path[i]].begin(), this->edges[path[i]].end(), i));
            }
            maxFlow += bottleneck;
            path = this->bfs(resNet);
        }
        if (saturation == maxFlow)
            return;
        else
        {
            vector<bool> visited(this->n, false);
            stack<int> traverse;
            traverse.push(0);
            while (!traverse.empty())
            {
                int i = traverse.top();
                traverse.pop();
                if (!visited[i])
                {
                    visited[i] = true;
                    for (int j = 0; j < this->edges[i].size(); j++)
                        if (resNet[i][edges[i][j]] > 0)
                            traverse.push(edges[i][j]);
                }
            }
            cout << this->teamNames[leftTeam] << " is eliminated.\nThey can win at most " << d.getWins()[leftTeam] << " + " << d.getLeft()[leftTeam] << " = " << d.getWins()[leftTeam] + d.getLeft()[leftTeam] << " games.\n";
            vector<int> positions;
            vector<string> tn = d.getTeamNames();
            int wins = 0, remain = 0;
            for (int i = this->ngames + 1; i < this->n - 1; i++)
                if (visited[i] && !visited[this->n - 1] && this->adjMat[i][this->n - 1])
                    positions.push_back(find(tn.begin(), tn.end(), this->nodes[i]) - tn.begin());
            cout << tn[positions[0]];
            for (int i = 0; i < positions.size(); i++)
            {
                wins += d.getWins()[positions[i]];
                if (i != 0 && i != positions.size() - 1)
                    cout << ", " << tn[positions[i]];
                else if (i != 0)
                    cout << " and " << tn[positions[i]];
            }
            sort(positions.begin(), positions.end());
            for (int i = 0; i < positions.size() - 1; i++)
            {
                for (int j = i + 1; j < positions.size(); j++)
                    remain += d.getGames()[positions[i]][positions[j]];
            }
            cout << " have won a total of " << wins << " games.\nThey play each other " << remain << " times.\nSo on average, each of the team wins " << wins + remain << "/" << positions.size() << " = " << float(wins + remain) / float(positions.size()) << " games.\n\n";
        }
    }
};

void eliminate(divison d, int n, int max, int min)
{
    vector<int> positions;
    for (int i = 0; i < n; i++)
    {
        if (d.getWins()[i] + d.getLeft()[i] < d.getWins()[max])
            cout << d.getTeamNames()[i] << " is eliminated.\nThey can win at most " << d.getWins()[i] << " + " << d.getLeft()[i] << " = " << d.getWins()[i] + d.getLeft()[i] << " games.\n" << d.getTeamNames()[max] << " has won a total of " << d.getWins()[max] << " games.\nThey play each other 0 times.\nSo on average, each of the teams in this group wins " << d.getWins()[max] << "/1 = " << d.getWins()[max] << " games.\n\n";
        else
        {
            flowNetwork fn(d, i);
            fn.maxFlowFordFulkerson(d);
        }
    }
}

int main()
{
    string filename;
    cout << "Enter File Name:\n";
    cin >> filename;
    vector<string> input = txtToString(filename);
    try
    {
        if (input.size() == 0)
            throw - 1;
        else if (input.size() == 1)
            throw 0;
        else
        {
            divison div(input);
            int maxpos = 0, minpos = 0, max = div.getWins()[0], min = div.getWins()[0];
            for (int i = 1; i < div.getNTeams(); i++)
            {
                if (div.getWins()[i] > max)
                {
                    max = div.getWins()[i];
                    maxpos = i;
                }
                if (div.getWins()[i] < min)
                {
                    min = div.getWins()[i];
                    minpos = i;
                }
            }
            eliminate(div, div.getNTeams(), maxpos, minpos);
        }
    }
    catch (int x)
    {
        if (x == -1)
            cout << __FILE__ << ": error: string array from input file was not created; check filename or contents of file again\n";
        else if (x == 0)
            cout << __FILE__ << ": error: the input text file has only one line; atleast two lines of input needed\n";
    }
}
