#include "algorithm.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  UC with Strict Expanded List
//
// Move Generator: U, R, D, L
//
////////////////////////////////////////////////////////////////////////////////////////////

// min heap by g: smaller getGCost() at the front of Q
bool compareGCost(Puzzle *a, Puzzle *b)
{
    return a->getGCost() > b->getGCost();
}

// Checks whether newMove would just undo lastMove (e.g. Up then Down).
bool isOppositeMove(char lastMove, char newMove)
{
    if (lastMove == 'U' && newMove == 'D')
        return true;
    if (lastMove == 'D' && newMove == 'U')
        return true;
    if (lastMove == 'L' && newMove == 'R')
        return true;
    if (lastMove == 'R' && newMove == 'L')
        return true;
    return false;
}

// Looks for a board string inside the frontier vector.
// Returns the index if found, or -1 if not present.
int findInQueue(vector<Puzzle *> &Q, string board)
{
    for (int i = 0; i < (int)Q.size(); i++)
    {
        if (Q[i]->getString() == board)
        {
            return i;
        }
    }
    return -1;
}

// Looks for a board string inside the expanded (closed) list.
bool isExpanded(vector<string> &expandedList, string board)
{
    for (int i = 0; i < (int)expandedList.size(); i++)
    {
        if (expandedList[i] == board)
        {
            return true;
        }
    }
    return false;
}

// Comparator for A* min-heap.
bool compareFCost(Puzzle *a, Puzzle *b) {
    // Smaller f-cost has higher priority
    if (a->getFCost() != b->getFCost()) {
        return a->getFCost() > b->getFCost();
    }

    // If f-cost is tied, prefer larger g-cost
    return a->getGCost() < b->getGCost();
}

string uc_explist(string const initialState, string const goalState, int &pathLength, int &numOfStateExpansions, int &maxQLength,
                  float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions)
{

    string path = "";
    clock_t startTime;
    numOfDeletionsFromMiddleOfHeap = 0;
    numOfLocalLoopsAvoided = 0;
    numOfAttemptedNodeReExpansions = 0;
    pathLength = 0;
    numOfStateExpansions = 0;
    maxQLength = 0;
    actualRunningTime = 0.0;
    startTime = clock();

    vector<Puzzle *> Q;
    vector<string> expandedList;
    Puzzle *start = new Puzzle(initialState, goalState);

    // check if the initial state is already at goalState
    if (start->goalMatch())
    {
        actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
        delete start;
        return "";
    }

    // push the start node onto the heap
    Q.push_back(start);
    make_heap(Q.begin(), Q.end(), compareGCost);
    maxQLength = 1;

    // while the heap is not empty
    while (!Q.empty())
    {
        // pop the node with the smallest g cost from the heap
        pop_heap(Q.begin(), Q.end(), compareGCost);
        Puzzle *current = Q.back();
        Q.pop_back();

        // get the current board string
        string currentBoard = current->getString();

        // Skip this node if we've already expanded this state before.
        if (isExpanded(expandedList, currentBoard))
        {
            numOfAttemptedNodeReExpansions++;
            delete current;
            continue;
        }

        // Goal check happens when we pop, not when we generate.
        if (current->goalMatch())
        {
            path = current->getPath();
            pathLength = path.size();
            delete current;
            break;
        }

        // add to expandedlist to avoid expanding again on the same node
        expandedList.push_back(currentBoard);
        numOfStateExpansions++;

        // get the parent path
        string parentPath = current->getPath();
        // get the last move
        char lastMove = '\0';
        // if the parent path is not empty, get the last move
        if (parentPath.size() > 0)
        {
            lastMove = parentPath[parentPath.size() - 1];
        }

        // Generate successors in fixed order: Up, Right, Down, Left.
        // skip if this move is illegal
        // skip if it undoes the last move (local loop)
        // create the child node
        // skip if board already expanded
        // if board is already in Q, keep cheaper g (or discard)
        // otherwise push child onto the heap
        // update max Q length

        // --- Up ---
        if (current->canMoveUp())
        {
            // skip if this move is illegal
            if (lastMove != '\0' && isOppositeMove(lastMove, 'U'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                // create the child node
                Puzzle *child = current->moveUp();
                string childBoard = child->getString();

                // skip if board already expanded
                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    // check if the child board is already in Q
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        // check if the child g cost is less than the existing node g cost
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            // delete the existing node
                            delete Q[existingIndex];
                            // remove the existing node from Q
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareGCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareGCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        // push the child node onto the heap
                        // update the heap
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareGCost);
                    }

                    // update max Q length
                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        // --- Right ---
        if (current->canMoveRight())
        {
            if (lastMove != '\0' && isOppositeMove(lastMove, 'R'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                Puzzle *child = current->moveRight();
                string childBoard = child->getString();

                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            delete Q[existingIndex];
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareGCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareGCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareGCost);
                    }

                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        // --- Down ---
        if (current->canMoveDown())
        {
            if (lastMove != '\0' && isOppositeMove(lastMove, 'D'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                Puzzle *child = current->moveDown();
                string childBoard = child->getString();

                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            delete Q[existingIndex];
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareGCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareGCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareGCost);
                    }

                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        // --- Left ---
        if (current->canMoveLeft())
        {
            if (lastMove != '\0' && isOppositeMove(lastMove, 'L'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                Puzzle *child = current->moveLeft();
                string childBoard = child->getString();

                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            delete Q[existingIndex];
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareGCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareGCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareGCost);
                    }

                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        delete current;
    }

    // delete all the nodes in the priority queue
    for (int i = 0; i < (int)Q.size(); i++) {
        delete Q[i];
    }

    actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
    return path;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A* with the Strict Expanded List
//
// Move Generator:
//
////////////////////////////////////////////////////////////////////////////////////////////
string aStar_ExpandedList(string const initialState, string const goalState, int &pathLength, int &numOfStateExpansions, int &maxQLength,
                          float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions, heuristicFunction heuristic)
{

    string path = "";
    clock_t startTime;

    numOfDeletionsFromMiddleOfHeap = 0;
    numOfLocalLoopsAvoided = 0;
    numOfAttemptedNodeReExpansions = 0;
    pathLength = 0;
    numOfStateExpansions = 0;
    maxQLength = 0;
    actualRunningTime = 0.0;
    startTime = clock();

    // initialize the priority queue and expanded list
    vector<Puzzle *> Q;
    vector<string> expandedList;

    // initialize the start node
    Puzzle *start = new Puzzle(initialState, goalState);
    start->updateHCost(heuristic);
    start->updateFCost();

    // check if the initial state is already at goalState
    if (start->goalMatch())
    {
        actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
        delete start;
        return "";
    }

    // push the start node onto the priority queue
    Q.push_back(start);
    make_heap(Q.begin(), Q.end(), compareFCost);
    maxQLength = 1;

    // while the priority queue is not empty
    while (!Q.empty())
    {
        // pop the node with the smallest f cost from the priority queue
        pop_heap(Q.begin(), Q.end(), compareFCost);
        // get the current node
        Puzzle *current = Q.back();
        Q.pop_back();

        // get the current board string
        string currentBoard = current->getString();

        // skip if the current node is already expanded

        if (isExpanded(expandedList, currentBoard))
        {
            numOfAttemptedNodeReExpansions++;
            delete current;
            continue;
        }

        // check if the current node is at the goalState
        if (current->goalMatch())
        {
            path = current->getPath();
            pathLength = path.size();
            delete current;
            break;
        }

        // add the current board to the expanded list
        expandedList.push_back(currentBoard);
        numOfStateExpansions++;

        // get the parent path and the last move
        string parentPath = current->getPath();
        char lastMove = '\0';
        if (parentPath.size() > 0)
        {
            lastMove = parentPath[parentPath.size() - 1];
        }

        // --- Up ---
        // skip if the last move is the opposite of the current move
        // create the child node
        // update the h cost and f cost of the child node
        // skip if the child board is already expanded
        // check if the child board is already in the priority queue
        // if it is, keep the cheaper g cost
        // otherwise, push the child node onto the priority queue
        // update the max Q length
        if (current->canMoveUp())
        {
            // skip if the last move is the opposite of the current move
            if (lastMove != '\0' && isOppositeMove(lastMove, 'U'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                // create the child node
                Puzzle *child = current->moveUp();
                string childBoard = child->getString();
                child->updateHCost(heuristic);
                child->updateFCost();

                // skip if the child board is already expanded
                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    // check if the child board is already in the priority queue
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        // check if the child g cost is less than the existing node g cost
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            // delete the existing node
                            delete Q[existingIndex];
                            // remove the existing node from the priority queue
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareFCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareFCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        // push the child node onto the priority queue
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareFCost);
                    }
                    
                    // update the max Q length
                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        // --- Right ---
        if (current->canMoveRight())
        {
            if (lastMove != '\0' && isOppositeMove(lastMove, 'R'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                Puzzle *child = current->moveRight();
                string childBoard = child->getString();
                child->updateHCost(heuristic);
                child->updateFCost();

                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            delete Q[existingIndex];
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareFCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareFCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareFCost);
                    }

                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        // --- Down ---
        if (current->canMoveDown())
        {
            if (lastMove != '\0' && isOppositeMove(lastMove, 'D'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                Puzzle *child = current->moveDown();
                string childBoard = child->getString();
                child->updateHCost(heuristic);
                child->updateFCost();

                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            delete Q[existingIndex];
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareFCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareFCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareFCost);
                    }

                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        // --- Left ---
        if (current->canMoveLeft())
        {
            if (lastMove != '\0' && isOppositeMove(lastMove, 'L'))
            {
                numOfLocalLoopsAvoided++;
            }
            else
            {
                Puzzle *child = current->moveLeft();
                string childBoard = child->getString();
                child->updateHCost(heuristic);
                child->updateFCost();

                if (isExpanded(expandedList, childBoard))
                {
                    numOfAttemptedNodeReExpansions++;
                    delete child;
                }
                else
                {
                    int existingIndex = findInQueue(Q, childBoard);
                    if (existingIndex != -1)
                    {
                        if (child->getGCost() < Q[existingIndex]->getGCost())
                        {
                            delete Q[existingIndex];
                            Q.erase(Q.begin() + existingIndex);
                            make_heap(Q.begin(), Q.end(), compareFCost);
                            numOfDeletionsFromMiddleOfHeap++;
                            Q.push_back(child);
                            push_heap(Q.begin(), Q.end(), compareFCost);
                        }
                        else
                        {
                            delete child;
                        }
                    }
                    else
                    {
                        Q.push_back(child);
                        push_heap(Q.begin(), Q.end(), compareFCost);
                    }

                    if ((int)Q.size() > maxQLength)
                    {
                        maxQLength = (int)Q.size();
                    }
                }
            }
        }

        delete current;
    }

    // delete all the nodes in the priority queue
    for (int i = 0; i < (int)Q.size(); i++) {
        delete Q[i];
    }

    actualRunningTime = ((float)(clock() - startTime) / CLOCKS_PER_SEC);
    return path;
}