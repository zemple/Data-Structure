/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
  if (first[curDim] == second[curDim]) {
    return first < second;
  } 
  return first[curDim] < second[curDim];
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
  int d1 = distance(target, currentBest, Dim);
  int d2 = distance(target, potential, Dim);
  if (d1 == d2) {
    return potential < currentBest;
  }
  return d2 < d1;
}

template <int Dim> 
int distance(const Point<Dim>& a, const Point<Dim>& b, int c) {
  int dis = 0;
  for (int i = 0; i < c; i++) {
    dis += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return dis;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
  size = 0;
  vector<Point<Dim>> points = newPoints;
  root = build(points, 0, 0, points.size() - 1);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::build(vector<Point<Dim>>& points, int d, unsigned int left, unsigned int right) {
  if (points.empty() || left > right || left < 0 || left >= points.size() || right < 0 || right >= points.size()) {
    return NULL;
  }
  int pivot = (right + left) / 2;
  int tmp = d % Dim;
  auto cmp = [tmp](auto l, auto r) {
    return smallerDimVal(l, r, tmp);
  };
  select(points.begin() + left, points.begin() + right + 1, points.begin() + pivot, cmp);
  KDTreeNode* subroot = new KDTreeNode(points[pivot]);
  size++;
  subroot->left = build(points, (d+1) % Dim, left, pivot - 1);
  subroot->right = build(points, (d+1) % Dim, pivot + 1, right);
 	return subroot;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy(root, other.root);
	size = other.size;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if (this != NULL) {
    clear(root);
  	copy(this->root, rhs.root);
  	size = rhs.size;
  }
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  clear(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
  Point<Dim> near = root->point;
  if (root->left == NULL && root->right == NULL) {
    return near;
  }
  findHelper(root, query, near, 0);
  return near;
}

template <int Dim>
void KDTree<Dim>::findHelper(KDTreeNode* node, const Point<Dim>& query, Point<Dim>& best, int curDim) const {
  if (node == NULL) {
    return;
  }
  bool flag = smallerDimVal(query, node->point, curDim);
  if (flag) {
    findHelper(node->left, query, best, (curDim + 1) % Dim);
  } else {
    findHelper(node->right, query, best, (curDim + 1) % Dim);
  }
  if (shouldReplace(query, best, node->point)) {
    best = node->point;
  }  
  double r = distance(best, query, Dim);
  double dis = query[curDim] - node->point[curDim];
  dis *= dis;
  if (dis <= r) {
    KDTreeNode* potential = flag ? node->right : node->left;
    findHelper(potential, query, best, (curDim + 1) % Dim);
  }
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
  while (start != end) {
    RandIter pivot = next(start, distance(start, end) / 2);
    RandIter left = start;
    RandIter right = end - 1;
    iter_swap(pivot, right);
    pivot = right;

    while (left < right) {
      if (cmp(*left, *pivot)) {
        left++;
      } else {
        right--;
        iter_swap(left, right);
      }
    }
    iter_swap(left, end - 1);

    if (k == left) {
      return;
    } else if (k > right) {
      start = next(left);
    } else {
      end = left;
    }
  }
}

template <int Dim>
void KDTree<Dim>::copy(KDTreeNode * subroot, KDTreeNode * other) {
	subroot = new KDTreeNode();
	subroot->point = other->point;
	copy(subroot->left, other->left);
	copy(subroot->right, other->right);
}

template <int Dim>
void KDTree<Dim>::clear(KDTreeNode * subroot) {
	if (subroot == NULL) {
    return;
  }
	if (subroot->left != NULL) {
    clear(subroot->left);
  }
	if (subroot->right != NULL) {
    clear(subroot->right);
  }
	delete subroot;
	subroot = NULL;
}


