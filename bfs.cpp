#include <algorithm>
#include <cstdint>
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

std::optional<uint16_t> bfs(std::span<std::vector<uint8_t>> maze, const std::pair<std::size_t, std::size_t> start,
                            const std::pair<std::size_t, std::size_t> finish)
{
    std::size_t n = maze.size();
    if (n == 0) return std::nullopt;
    std::size_t m = maze[0].size();

    std::queue<std::pair<std::size_t, std::size_t>> q;
    q.push(start);

    constexpr const int dx[] = {-1, 1, 0, 0};
    constexpr const int dy[] = {0, 0, -1, 1};
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    visited[start.first][start.second] = true;
    uint16_t steps = 0;

    while (!q.empty())
    {
        auto current = q.front();
        q.pop();
        if (current == finish) return steps;
        for (std::size_t i = 0; i < 4; ++i)
        {
            std::size_t nx = current.first + dx[i];
            std::size_t ny = current.second + dy[i];

            if (nx < m && ny < n && !visited[ny][nx] && maze[ny][nx] != 1)
            {
                maze[ny][nx] = 2;
                visited[ny][nx] = true;
                q.emplace(nx, ny);
            }
        }
        ++steps;
    }
    return std::nullopt;
}

int main()
{
    try
    {
        std::size_t n, m;
        std::cout << "Enter number of row (n > 0).\n";
        if (!(std::cin >> n) || n <= 0) throw std::invalid_argument("Invalid value for N: must be integer > 0.");
        std::cout << "Enter number of columns (m > 0).\n";
        if (!(std::cin >> m) || m <= 0) throw std::invalid_argument("Invalid value for M: must be integer > 0.");

        std::vector<std::vector<uint8_t>> maze(n, std::vector<uint8_t>(m));

        std::cout << "Enter maze (1 - wall, 0 - empty field.\n";
        for (std::size_t i = 0; i < n; ++i)
        {
            for (std::size_t j = 0; j < m; ++j)
            {
                if (!(std::cin >> maze[i][j])) throw std::invalid_argument("Invalid maze`s element.");
            }
        }

        auto path_length = bfs(maze, {0, 0}, {n - 1, m - 1});

        if (!path_length)
        {
            std::cerr << "Path not exists.\n";
            return EXIT_FAILURE;
        }

        std::cout << "\nPath length: " << *path_length << "\n";
        for (auto const& line : maze)
        {
            for (auto const el : line)
            {
                std::cout << el << " ";
            }
            std::cout << "\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Catched error: " << e.what() << "\n";
    }
    return EXIT_SUCCESS;
}