#ifndef BFS_H
#define BFS_H
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
using namespace std;

const int MAX_AIRPORTS = 50;
//Airport info structure
struct Airport 
{
    string name;
    string city;
    string country;
    bool emergency;
};
//Graph for trip-planning
class Graph
{
private:
    Airport airports[MAX_AIRPORTS];
    bool adj[MAX_AIRPORTS][MAX_AIRPORTS];
    int n;
public:
    Graph() : n(0) 
    {
        for (int i = 0; i < MAX_AIRPORTS; i++) 
            for (int j = 0; j < MAX_AIRPORTS; j++)
                adj[i][j] = false;
    }
    //loading Airports data from airport.txt file
    void loadAirports(const string& fname)
    {
        ifstream in(fname.c_str());
        if (!in) return;
        n = 0;
        while (true)
        {
            Airport a; int em;
            if (!(in >> a.name >> a.city >> a.country >> em)) 
                break;
            a.emergency = (em != 0);
            airports[n++] = a;
            if (n >= MAX_AIRPORTS)
                break;
        }
        in.close();
    }
    //loading routes from route.txt file
    void loadRoutes(const string& fname)
    {
        ifstream in(fname.c_str());
        if (!in) return;
        int u, v;
        while (in >> u >> v) 
        {
            if (u >= 0 && u < n && v >= 0 && v < n) adj[u][v] = true;
        }
        in.close();
    }
    //display all the airports
    void displayAirports()
    {
        cout << "Index\tCity\tProvince\tCountry\tEmergency\n";
        cout << "==================================================\n";
        for (int i = 0; i < n; i++)
        {
            cout << i << "\t" << airports[i].name << "\t" << airports[i].city
                << "\t" << airports[i].country << "\t" << (airports[i].emergency ? "Yes" : "No") << "\n";
        }
    }
    //Algorithm of BFS for the shortest path calculation
    void BFS(int s, int d) 
    {
        if (s < 0 || s >= n || d < 0 || d >= n) 
        { 
            cout << "Invalid indices\n"; 
            return;
        }
        int dist[MAX_AIRPORTS];
        int parent[MAX_AIRPORTS]; 
        bool vis[MAX_AIRPORTS];
        for (int i = 0; i < n; i++) 
        {
            dist[i] = -1; 
            parent[i] = -1;
            vis[i] = false;
        }
        queue<int> q;
        vis[s] = true; 
        dist[s] = 0;
        q.push(s);
        while (!q.empty()) 
        {
            int u = q.front();
            q.pop();
            if (u == d) 
                break;
            for (int v = 0; v < n; ++v)
            {
                if (adj[u][v] && !vis[v])
                {
                    vis[v] = true;
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        //displaying bfs table 
        cout << "Airport\tDistance\tParent\n";
        for (int i = 0; i < n; i++) 
        {
            cout << airports[i].name << "\t" << dist[i] << "\t";
            if (parent[i] == -1) 
                cout << "None\n";
            else 
                cout << airports[parent[i]].name << "\n";
        }
        if (dist[d] == -1) 
        {
            cout << "No route from " << airports[s].name << " to " << airports[d].name << "\n";
            return;
        }    
        cout << "Minimum stops from " << airports[s].name << " to " << airports[d].name << ": " << dist[d] << "\n";
        int path[MAX_AIRPORTS]; 
        int len = 0;
        int cur = d;
        while (cur != -1)
        {
            path[len++] = cur; 
            cur = parent[cur]; 
        }
        cout << "Route: ";
        for (int i = len - 1; i >= 0; --i)
        {
            cout << airports[path[i]].name;
            if (i) cout << " -> ";
        }
        cout << "\n";
    }
};

#endif 
