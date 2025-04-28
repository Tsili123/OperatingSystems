// exponential_distribution
#include <iostream>
#include <random>

int exp_rand(double avg) {
    // Generate a random number in [0, 1)
    double randnum = (double) rand() / ((double) RAND_MAX + 1.0);
    // Lambda is 1 / average and the formula for  a random
    // exponentially distributed number is
    // double exp_randnum = (-1) * log(1 - randnum) / lambda
    // and since the function input is the average,
    double exp_randnum = (-1) * log(1 - randnum) * avg;
    double for_round = exp_randnum + 0.5;
    int output = (int)for_round;
    return output;
}

int main()
{
  const int nrolls=1000;  // number of experiments
  const int nstars=100;    // maximum number of stars to distribute
  const int nintervals=10; // number of intervals

  std::default_random_engine generator;
  std::exponential_distribution<> distribution(0.005);

  int p[nintervals]={};

  for (int i=0; i<nrolls; ++i) {
    double number = distribution(generator);
    std::cout << number << std::endl;
    //if (number<1.0) ++p[int(nintervals*number)];
  }

//  std::cout << "exponential_distribution (3.5):" << std::endl;
//  std::cout << std::fixed; std::cout.precision(1);
//
//  for (int i=0; i<nintervals; ++i) {
//    std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": ";
//    std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
//  }

  return 0;
}