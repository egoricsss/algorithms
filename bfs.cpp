#include <algorithm>
#include <iostream>
#include <optional>
#include <queue>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

unsigned int bfs(std::span<const std::vector<std::size_t>> adj, std::size_t source)
{
    std::queue<std::size_t> q;
    q.push(source);
    std::vector<bool> visited(adj.size(), false);
    visited[source] = true;

    while (!q.empty())
    {
        std::size_t front = q.front();
        q.pop();
        for (auto v : adj[front])
        {
            if (!visited[v])
            {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return std::count(visited.begin(), visited.end(), true);
}

std::optional<std::size_t> bfs(std::span<const std::vector<int>> maze, const std::pair<std::size_t, std::size_t>& start,
                               const std::pair<std::size_t, std::size_t>& finish)
{
    std::size_t n = maze.size();
    std::size_t m = maze[0].size();

    std::queue<std::pair<std::size_t, std::size_t>> q;
    q.push(start);

    constexpr const int dx[] = {-1, 1, 0, 0};
    constexpr const int dy[] = {0, 0, -1, -1};
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, -1));
    // -1 - means, that not visited,
    // 1 - wall,  0 - visited
    dist[start.first][start.second] = 0;

    while (!q.empty())
    {
        auto current = q.front();
        q.pop();
        if (current == finish) return dist[current.first][current.second];
        for (std::size_t i = 0; i < 4; ++i)
        {
            int nx = current.first + dx[i];
            int ny = current.second + dy[i];

            if (nx >= 0 && nx < m && ny >= 0 && ny < n && maze[ny][nx] != 1 && maze[ny][nx] == -1)
            {
                dist[ny][nx] += dist[current.first][current.second] + 1;
                q.push({nx, ny});
            }
        }
    }
    return std::nullopt;
}

int main()
{
    std::size_t n, m;
    std::cout << "Enter number of row (n > 0).\n";
    if (!(std::cin >> n) || n <= 0) throw std::invalid_argument("Invalid value for N: must be integer > 0.");
    std::cout << "Enter number of columns (m > 0).\n";
    if (!(std::cin >> m) || m <= 0) throw std::invalid_argument("Invalid value for M: must be integer > 0.");

    std::vector<std::vector<int>> maze(n, std::vector<int>(m));

    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < m; ++j)
        {
            if (!(std::cin >> maze[i][j])) throw std::invalid_argument("Invalid maze`s element.");
        }
    }

    auto path_length = bfs(maze, {0, 0}, {n - 1, m - 1});

    if (!path_length) throw std::runtime_error("Path not exists.");

    for (auto const& line : maze)
    {
        for (auto const el : line)
        {
            std::cout << el << " ";
        }
        std::cout << "\n";
    }
    return 0;
}