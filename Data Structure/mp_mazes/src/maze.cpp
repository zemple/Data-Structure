/* Your code here! */
#include "maze.h"

SquareMaze::SquareMaze() {
    w = 0;
    h = 0;
}

void SquareMaze::makeMaze(int width, int height) {
    w = width;
    h = height;
    int size = width * height;
    visited.resize(size);
    ds.addelements(size);
    for (int i = 0; i < size; i++) {
        walls.push_back(pair<bool, bool>(true, true));
        visited[i] = false;
    }
    for (int i = 0; i < size; i++) {
        vector<int> vec;
        if (((i + 1) % width != 0) && (ds.find(i) != ds.find(i+1))) {
			vec.push_back(0);
		}
		if ((i < (width * (height - 1))) && (ds.find(i) != ds.find(i+width))) {
			vec.push_back(1);
		}
		if ((i % width != 0) && (ds.find(i) != ds.find(i-1))) {
			vec.push_back(2);
		}
		if ((i >= width) && (ds.find(i) != ds.find(i-width))) {
			vec.push_back(3);
		}
        if (vec.empty()) {
            continue;
        }
        int r = vec[rand() % vec.size()];
        if (r == 0) {
			ds.setunion(i, i+1);
			(walls)[i].first = false; 
		} else if (r == 1) {
			ds.setunion(i, i+width);
			(walls)[i].second = false;
		} else if (r == 2) {
			ds.setunion(i, i-1);
			(walls)[i-1].first = false;
		} else if (r == 3) {
			ds.setunion(i, i-width);
			(walls)[i-width].second = false;
		}
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    switch (dir) {
        case 0:
            if (x >= w - 1) {
                return false;
            }
		    return !(walls)[x + y * w].first;
        case 1:
            if (y >= h - 1) {
                return false;
            }
            return !(walls)[x + w * y].second;
        case 2:
            if (x <= 0) {
                return false;
            }
		    return !(walls)[x - 1 + y * w].first;
        case 3:
            if (y <= 0) {
                return false;
            }
		    return !(walls)[x + (y - 1) * w].second;
    }
    return false;
}

 void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (dir == 0) {
        walls[x + w * y].first = exists;
    } 
    walls[x + w * y].second = exists;
 }

vector<int> SquareMaze::solveMaze() {
    stack<int> s;
	int* dist = new int[w * h];
	if(!visited[0]) {
		for(int i = 0; i < w * h;i++) {
			visited[i] = false;
		}
	}
	s.push(0);
	Distance(s, dist);
	int i = w * h - w;
	int max_dist = i;
	for(; i < w * h; i++)
	{
		if(dist[i] > dist[max_dist])
		{
			max_dist = i;
		}
	}
	vector<int> vec;
	int d = max_dist;
	while(d != 0) {
		if(canTravel(d % w, d / w, 0) && dist[d] - dist[d + 1] == 1) {
			vec.insert(vec.begin(), 2);
			d = d + 1;
		}
		if(canTravel(d % w, d / w, 1) && dist[d] - dist[d + w] == 1) {
			vec.insert(vec.begin(), 3);
			d = d + w;
		}
		if(canTravel(d % w, d / w, 2) && dist[d] - dist[d - 1] == 1) {
			vec.insert(vec.begin(), 0);
			d = d - 1;
		}
		if(canTravel(d % w, d / w, 3) && dist[d] - dist[d - w] == 1) {
			vec.insert(vec.begin(), 1);
			d = d - w;
		}
	}
	delete[] dist;
	return vec;
 }

 cs225::PNG* SquareMaze::drawMaze() const {
    int new_w = w * 10 + 1;
    int new_h = h * 10 + 1;
    PNG* maze = new PNG(new_w, new_h);
    for (int x = 10; x < new_w; x++) {
        maze->getPixel(x, 0) = HSLAPixel(0, 0, 0);
    } 
    for (int y = 0; y < new_h; y++) {
        maze->getPixel(0, y) = HSLAPixel(0, 0, 0); 
    }
  
    for (int y = 0; y < h; ++ y) {
        for (int x = 0; x < w; ++ x) {
            if (!canTravel(x, y, 0)) {
                for (int i = 0; i <= 10; i++) {
                    maze->getPixel((x + 1) * 10, y * 10 + i) = HSLAPixel(0, 0, 0);
                }
            }
            if (!canTravel(x, y, 1)) {
                for (int i = 0; i <= 10; i++) {
                    maze->getPixel(x * 10 + i, (y + 1) * 10) = HSLAPixel(0, 0, 0);
                }
            }  
        }
    }
    return maze;
 }

cs225::PNG* SquareMaze::drawMazeWithSolution() {
	cs225::PNG* maze = drawMaze();
	vector<int> steps = solveMaze();
	int x = 5; 
	int y = 5; 
	for (size_t j = 0; j < steps.size(); j++) {
		if (steps[j] == 0) {
			for (int i = 0; i <= 10; i++) {
				maze->getPixel(x + i, y).h = 0;
				maze->getPixel(x + i, y).s = 1;
				maze->getPixel(x + i, y).l = 0.5;
				maze->getPixel(x + i, y).a = 1.0;
			}
			x += 10;
		} else if (steps[j] == 1) {
			for (int i = 0; i <= 10; i++) {
				maze->getPixel(x, y + i).h = 0;
				maze->getPixel(x, y + i).s = 1;
				maze->getPixel(x, y + i).l = 0.5;
				maze->getPixel(x, y + i).a = 1.0;
			}
			y += 10;
		} else if (steps[j] == 2) {
			for (int i = 0; i <= 10; i++) {
			maze->getPixel(x - i, y).h = 0;
			maze->getPixel(x - i, y).s = 1;
			maze->getPixel(x - i, y).l = 0.5;
			maze->getPixel(x - i, y).a = 1.0;
			}
			x -= 10;
		} else if (steps[j] == 3) {
			for (int i = 0; i <= 10; i++) {
				maze->getPixel(x, y - i).h = 0;
				maze->getPixel(x, y - i).s = 1;
				maze->getPixel(x, y - i).l = 0.5;
				maze->getPixel(x, y - i).a = 1.0;
			}
			y -= 10;
		}
	}
	int new_w = x / 10;
	for (int i = 1; i < 10; i++) {
		maze -> getPixel(new_w * 10 + i, h * 10).l = 1;
		maze -> getPixel(new_w * 10 + i, h * 10).a = 1;
	}
	return maze;
}
 
void SquareMaze::Distance(stack<int> s, int* dist) {
	int step = 0;
	dist[0] = 0;
	int d;
	if(!visited[0]) {
		visited[0] = !visited[0];
		while(!s.empty()) {
			d = s.top();
			s.pop();
			step = dist[d];
			if(canTravel(d % w, d / w, 0) && !visited[d + 1])
			{
				dist[d + 1] = step + 1;
				visited[d + 1] = !visited[d + 1];
				s.push(d + 1);
			}
			if(canTravel(d % w, d / w, 1) && !visited[d + w])
			{
				dist[d + w] = step + 1;
				visited[d + w] = !visited[d + w];
				s.push(d + w);
			}
			if(canTravel(d % w, d / w, 2) && !visited[d - 1]) {
				dist[d - 1] = step + 1;
				visited[d - 1] = !visited[d - 1];
				s.push(d-1);
			}
			if(canTravel(d % w, d / w, 3) && !visited[d - w])
			{
				dist[d - w] = step + 1;
				visited[d - w] = !visited[d - w];
				s.push(d - w);
			}
		}
	}
	if(visited[0]) {
		visited[0] = !visited[0];
		while(!s.empty()) {
			d = s.top();
			s.pop();
			step = dist[d];
			if(canTravel(d % w, d / w, 0) && visited[d + 1])
			{
				dist[d + 1] = step + 1;
				visited[d + 1] = !visited[d + 1];
				s.push(d + 1);
			}
			if(canTravel(d % w, d / w, 1) && visited[d + w])
			{
				dist[d + w] = step + 1;
				visited[d + w] = !visited[d + w];
				s.push(d + w);
			}
			if(canTravel(d % w, d / w, 2) && visited[d - 1]) {
				dist[d - 1] = step + 1;
				visited[d - 1] = !visited[d - 1];
				s.push(d-1);
			}
			if(canTravel(d % w, d / w, 3) && visited[d - w])
			{
				dist[d - w] = step + 1;
				visited[d - w] = !visited[d - w];
				s.push(d - w);
			}
		}
	}
 }

