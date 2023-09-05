int TotalCost = 100, Route[56];
int const N = 8, M = 7;

//actual map of the city
int CityMap[N][M] = {
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, 0, -1, 8, -1, -1 },
  { -1, 4, 0, 7, 0, 12, -1 },
  { -1, -1, 3, -1, 11, -1, -1 },
  { -1, -1, 6, -1, 10, -1, -1 },
  { -1, 2, 0, 0, 0, 9, -1 },
  { -1, -1, 1, -1, 5, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 }
};


//-1 means not visited yet
int visited[N][M] = {
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
};


//default answer
int shortestPath[N][M] = {
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1 },
};


//update the shortest path with new best path
void updateShortestPath() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      shortestPath[i][j] = visited[i][j];
    }
  }
}

int avail(int x, int y) {
  if (x < 0 || x >= N || y < 0 || y >= M || CityMap[x][y] == -1) {
    return 0;
  }
  return 1;
}


//Find the best path
void FindPath(int x, int y, int cost, int ep) {
  if (CityMap[x][y] == ep) {
    if (cost <= TotalCost) {
      updateShortestPath();
    } else {
      return;
    }
  }

  if (visited[x][y] == -1)
    visited[x][y] = 1;
  //Go up
  if (avail(x - 1, y))
    FindPath(x - 1, y, cost + 1, ep);
  //Go down
  if (avail(x + 1, y))
    FindPath(x + 1, y, cost + 1, ep);
  //go Right
  if (avail(x, y - 1)) {
    FindPath(x, y - 1, cost + 1, ep);
  }
  //go left
  if (avail(x, y + 1)) {
    FindPath(x, y + 1, cost + 1, ep);
  }
  return;
}


//For getting the string for navigation code
String getPath(){
  String shortPath="SSSSSSSSSSSSSSSSSSSS";
  int k=0;
  for(int i=N; i>=0; i--){
    for(int j=M; j>=0; j--){
      if(shortestPath[i][j]!=-1){
        shortPath[k]='F';
        k++;
      }
    }
  }
  return shortPath;
}

int startX = 2, startY = 6, cost = 0;
String solvePath(int endPoint) {
  FindPath(startX, startY, cost, endPoint);
  return getPath();
}
