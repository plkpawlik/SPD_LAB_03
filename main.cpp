#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;


/*  Declarations
/*   *   *   *   *   *   *   *   *   *   */

typedef struct {

    vector<int> p;

    int sum_p = 0;
    int num_x = 0;

} task_t;

int cmax( int m, int n, vector<task_t> &tasks );
int exec( int m, int n, vector<task_t> &tasks );

void sortTasks( vector<task_t> &tasks );


/*  Executor
/*   *   *   *   *   *   *   *   *   *   */
int main( int argc, char **argv ) {

    //  initial dataset
    ifstream data( "neh.data.txt" );
    int num_m;	//	num machines
    int num_n;	//	num tasks

	//  read defer buffer
	string defer;
	int expected;

	// 	for printing
	bool print = ( bool )( argv[1] == "print" );


	//  process for all 4 datasets
    for ( int i = 0; i <= 120; i++ ) {

		/*  Process stream
	    /*   *   *   *   *   *   *   *   */

		//  create new file header
        string fileHeader = "data." + ( string )(( i < 10 ) ? "0" : "" ) + ( string )(( i < 100 ) ? "0" : "" ) + to_string( i ) + ":";

	    //  read till heder found
	    do {
	        data >> defer;
	    } while( defer != fileHeader );


	    /*  Read data
	    /*   *   *   *   *   *   *   *   */

	    // 	read N & M values
	    data >> num_n >> num_m;

		// 	task dataset
	    vector<task_t> tasks( num_n );  //  initialize vector of taks
	    int buffer;                     //  buffer for p values

	    // 	read tasks
	    for( int i = 0; i < num_n; i++ ) {
	        for( int j = 0; j < num_m; j++ ) {

	            //  read data into buffer
	            data >> buffer;

	            //  push data into stack
	            tasks[i].p.push_back( buffer );

				// 	update task sum p
				tasks[i].sum_p += buffer;
	        }

	        //  set task index
	        tasks[i].num_x = i + 1;
	    }

		//  read till "neh:" apears
	    do {
	        data >> defer;
	    } while( defer != "neh:" );

		// 	read expected value
		data >> expected;

	    /*  Execute NEH algorithm
	    /*   *   *   *   *   *   *   *   */

		// 	print restul value for current run
		cout << fileHeader << ": expected: " << setw( 6 ) << expected << " is : " << exec( num_m, num_n, tasks ) << endl;

		// 	print result order
	    for( int i = 0; i < num_n && print; i++ ) {

			// 	print task info
			cout << setw( 4 ) << tasks[i].num_x << ". [ cost: " << setw( 5 ) << tasks[i].sum_p << " ] ";

	        for( int j = 0; j < num_m; j++ ) {

				// 	print task details
				cout << setw( 3 ) << tasks[i].p[j] << " ";
	        }

			// 	break line
			cout << endl;
	    }
		
	}

    // 	safe exit
    return 0;
}


/*  Solutions
/*   *   *   *   *   *   *   *   *   *   */


int cmax( int m, int n, vector<task_t> &tasks ) {

	// 	initial dataset
	int t[ n + 1 ][ m + 1 ];

	// 	set
	memset( t, 0, sizeof( t ));

	// 	find result
	for( int i = 1; i < n + 1; i++ ) {
		for( int j = 1; j < m + 1; j++ ) {
			t[i][j] = max( t[i - 1][j], t[i][j - 1]) + tasks[i - 1].p[j - 1];
		}
	}

	// 	return result
	return t[n][m];
}

int exec( int m, int n, vector<task_t> &tasks ) {

    //  initial sort
    sortTasks( tasks );

	// 	strict sort
	for( int i = 0; i < n; i++ ) {

		// 	initial values
		int c_max = 999999;
		int p_max = 0;
		
		for( int j = i; j >= 0; j-- ) {

			// 	calc partial result
			int curr = cmax( m, i + 1, tasks );

			// 	test parital result
			if( c_max >= curr ) {
				c_max = curr;
				p_max = j;
			}

			// 	swap if needed
			if( j > 0 ) {
				swap( tasks[j], tasks[j - 1] );
			}
		}

		for( int j = 0; j < p_max; j++ ) {
			swap( tasks[j], tasks[j + 1]);
		}
	}

	// 	return final result
	return cmax( m, n, tasks );
}

void sortTasks( vector<task_t> &tasks ) {
    sort( tasks.begin(), tasks.end(), []( task_t const &p, task_t const &q ) -> bool {
        return p.sum_p > q.sum_p;
    });
}