#include "mmt_graph.h"
#include "mmt_partial_coloring.h"

#include <time.h>
#include <vector>
#include <utility>

class MMT {
public:
  MMT(int argc, char **av, int L, int T, int time_limit_sec, int pool_size = 99, double pGreedy = 0.5) : graph(argc,av), L(L), T(T), time_limit_sec(time_limit_sec), pool_size((pool_size/3)*3), cur_best_coloring(MMTPartialColoring(graph.n, &graph, L, T)), pGreedy(pGreedy) {
    // compute lower bound
    // compute upper bound
    UB = graph.n + 1;
    LB = 9;
  }

  void testing(){
    MMTPartialColoring A = MMTPartialColoring(7, &graph, L, T);
    MMTPartialColoring B = MMTPartialColoring(7, &graph, L, T);
    MMTPartialColoring C = MMTPartialColoring(7, &graph, L, T);

    A.tabuSearch();
    A.toString();
    B.tabuSearch();
    B.toString();
    C.crossover(&A, &B);
    C.lockColoring();
    C.toString();
  }

  void EAOptimizer() {
    clock_t t = clock();
    while (((float) clock() - t)/CLOCKS_PER_SEC < time_limit_sec && UB > LB) {
      EADecision(UB);
      cur_best_coloring.toString();
      UB--;
    }
    std::cout << "solution found ? " << '\t';
    UB == LB ? std::cout << "YAAY in " << ((float) clock() - t)/CLOCKS_PER_SEC << " secs" << '\n' : std::cout << "no we timed out :o" << '\n';
  }

  void EADecision(int UB) {
    // generate initial pool of solutions
    std::vector<MMTPartialColoring> pool(pool_size, MMTPartialColoring(UB-1, &graph, L, T));
    for (size_t i = 0; i < pool_size - 2; ) {
      if(pool.at(i).greedy()) {
        cur_best_coloring = pool[i];
        return;
      }
      i++;
      if(pool.at(i).dsatur()) {
        cur_best_coloring = pool[i];
        return;
      }
      i++;
      if(pool.at(i).tabuSearch()) {
        cur_best_coloring = pool[i];
        return;
      }
      i++;
    }

    clock_t t = clock();
    while (((float) clock() - t)/CLOCKS_PER_SEC < time_limit_sec) {
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::shuffle(pool.begin(), pool.end(), std::default_random_engine(seed));

      MMTPartialColoring crossover_child(UB-1, &graph, L, T);
      // generate offspring and if it is not already a solution improve by calling tabuSearch on it
      if(crossover_child.crossover(&pool[0], &pool[1]) || crossover_child.tabuSearch()) {
        cur_best_coloring = crossover_child;
        return;
      }

      /*

      priority greedy needs to be called here
      not implemented yet

        crossover_child similar to a solution in the pool? trigger priorityGreedy
        offspring is considered similar to another partial coloring if each
        fitness and #uncolered vertices are equal

      */

      printf("%d,\t", crossover_child.evaluate());
      // for (const auto & u : crossover_child.uncolored) {
      //   std::cout << u << '\t';
      // }
      // std::cout << '\n';
      // delete worst parent and insert child to pool
      if (pool[0].evaluate() <= pool[1].evaluate()) {
        pool[1] = crossover_child;
      } else {
        pool[0] = crossover_child;
      }
    }
  }

  MMTPartialColoring* getColoring(){
    return &cur_best_coloring;
  }

private:
  int L, T, time_limit_sec, pool_size;
  MMTGraph graph;
  int UB, LB;
  MMTPartialColoring cur_best_coloring;
  double pGreedy;
};

int main(int argc, char **av) {

  MMT mmt(argc, av, /*L*/ 1000,/*T*/ 100, /*time limit*/ 60, /*pool size*/ 99);

  mmt.EAOptimizer();
  // mmt.testing();

  return 0;
}
