#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

template <class type = double> class Array {
    public:
        /*
            constructor  1 ..
            default constructor - has no parameters
            buil Matrix[1][1]
        */
        Array (  ): Cell(new type *[1]), row(1), col(1) { this->Cell[0] = new type [1];}

        /*
            constructor  2 ..
            build Matrix[argrow][argcol]
        */
        Array(int argrow, int argcol): Cell(new type *[row]), row(argrow), col(argcol) {
            for (int i = 0; i < this->row; i++)
                this->Cell[i] = new type [this->col];
        }

        /*
            constructor  3 ..
            copy constructor - Deep Constructor
            other -> this
        */
        Array (const Array < type > &other) {
            this->row = other.row;
            this->col = other.col;
            this->Cell = new type *[this->row];
            for (int i = 0; i < this->row; i++) {
                this->Cell[i] = new type [this->col];
                for (int j = 0; j < this->col; j++)
                this->Cell[i][j] = other.Cell[i][j];
            }
        }

        /*
            distructor  1 ..
            delete field Cell
        */
        ~ Array (  ) {
            delete [  ] this->Cell;
        }

        /* Operators Overloadings */

        /*
            &[ ] operator overload
            get index (field &Cell[ argrow ]) with reference
        */
        inline type* &operator [ ](int argrow) { return this->Cell[argrow]; }

        /*
            Multiplication * opMultiplicationerator overload
            return (this * other) : tow matrices
        */
        Array < type > operator *(Array < type > other) {
            Array < type > _result(this->row, other.col);

            if (this->col == other.row) {
                for (int i = 0; i < _result.row; i++)
                    for (int j = 0; j < _result.col; j++) {
                        _result[i][j] = 0.0;
                        for (int k = 0; k < this->col; k++)
                            _result[i][j] += this->Cell[i][k] * other.Cell[k][j];
                        }
                return _result;
            }
        }

        /* Algebra Methods */

        // return minor of this
        Array <type> minor(int x, int y) {
            int x1 = 0, y1 = 0;
            Array <type> _result(this->row - 1, this->col - 1);

            for (int i = 0; i < this->row; i++) {
                if (i == x) {
                    x1 = 1;
                    continue;
                }

                for (int j = 0; j < this->col; j++) {
                    if (j == y) {
                        y1 = 1;
                        continue;
                    }
                    _result[i - x1][j - y1] = this->Cell[i][j];
                }
                y1 = 0;
            }
            return _result;
        }
        
    private:
        class POINT{
            public:
                char option;
                int  coordinates;
                POINT(char op, int co) :option(op), coordinates(co) {}
        };
        
    public:
        // return determinant of this
        inline long double determinant(  ) {
            POINT coor = bestChoice();
            long double sum = 0.0;
            if (coor.option == 'r')
                if (this-> row == 1)
                    return this->Cell[0][0];
                else {
                    for (int c = 0; c < this->col; c++)
                        if (this->Cell[coor.coordinates][c])
                            sum += ((coor.coordinates + c) % 2 ? -1:1) * this->Cell[coor.coordinates][c] * this->minor(coor.coordinates, c).determinant();
                }
            else
                if (this->col == 1)
                    return this-> Cell[0][0];
                else {
                    for (int r = 0; r < this->row; r++)
                        if (this->Cell[r][coor.coordinates])
                            sum += ((coor.coordinates + r) % 2 ? -1:1) * this->Cell[r][coor.coordinates] * this->minor(r, coor.coordinates).determinant();
                }
            return sum;
        }

        /*
            search for the line or column that
            has most ZERO element
        */
        POINT bestChoice (  ) {
            int RowZeroCount = 0, _row = 0;
            for (int i = 0; i < this->row; i++) {
                int temp = 0;
                for (int j = 0; j < this->col; j++)
                    temp += this->Cell[i][j] ? 0 : 1;
                RowZeroCount = temp > RowZeroCount ? temp : RowZeroCount;
                _row = temp > RowZeroCount ? i : _row;
            }

            int ColZeroCount = 0, _col = 0;
            for (int j = 0; j < this->col; j++) {
                int temp = 0;
                for (int i = 0; i < this->row; i++)
                    temp += this->Cell[i][j] ? 0 : 1;
                ColZeroCount = temp > ColZeroCount ? temp : ColZeroCount;
                _col = temp > ColZeroCount ? j : _col;
            }

            return RowZeroCount > ColZeroCount ? POINT('r', _row) : POINT('c', _row);
        }

        // return transported of this
        inline Array < type > transport (  ) {
        Array < type > temp(this->col, this->row);
            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    temp[j][i] = this->Cell[i][j];
            return temp;
        }

        // return adjacent of this
        inline Array < type > adj (  ) {
            Array < type > temp(this->row, this->col);

            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    temp.Cell[i][j] = (((i + j + 2) % 2) ? 1 : -1) * this->minor(i, j).determinant();

            return temp.transport();
        }

        // return invert of this = (this ^ -1)
        inline Array < type > invers (  ) {
        Array < type > temp( this->adj ( ) );

            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    temp.Cell[i][j] *= -1.0/this->determinant();

            return temp;
        }

        /*
            display this
            x	y	z
            a	b	c
            d	e	f
        */
        bool display (  ) {
            if (this->row < 1 || 1 > this->col)
                return 0;
            else {
                for (int i = 0; i < this->row; i++) {
                    for (int j = 0; j < this->col; j++)
                        cout << this->Cell[i][j] << '\t';
                    cout << '\n';
                }
                cout << '\n';
            }
            return 1;
        }

        // read this
        bool read (  ) {
            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    cin >> this->Cell[i][j];
            return 1;
        }

        void readEquation( ) {
            char constant = 'a', var = 't';
            cout << "input each equation like : ";
            for (int i = 0; i < (this->col - 1); i++) cout << constant++ << "." << var++ << "   ";
            cout << "= Y" << endl;

            for (int i = 0; i < this->row; i++)
                for (int j = 0; j < this->col; j++)
                    cin >> Cell[i][j];
        }

        void writeEquation( ) {
            char constant = 't';

            cout << "Equations System" << endl;
            for (int i = 0; i < this->row; i++) {
                for (int j = 0; j < (this->col - 1); j++) cout << Cell[i][j] << constant++ << "   ";
                cout << "=\t" << Cell[i][this->col - 1] << endl;
                constant = 't';
            }
            cout << endl;
        }

        /*   Methods.Get.Set
         *   ...  @protected fields
         */

        void SetRow(int argrow) {
            delete [] this->Cell;
            this->Cell = new type *[this->row = argrow];
        }

        void SetCol(int argcol) {
            for (int i = 0; i < this->row; i++) this->Cell[i] = new type [this->col = argcol];
        }

        int Row (  ) { return this->row; }
        int Col (  ) { return this->col; }

    private:
        int row, col;
        type **Cell;

};

/* Invert's Theory : to solv System of Equations */
template <class type>  inline Array < type > InvertSolutions(Array < type >);

int main(int argc, char const *argv[]) {

    unsigned char option ( 0 );
    cout	<< "What would you want to try {1 or 2} ?"
        << "\n\t1 . Arrays Informations"
        << "\n\t2 . System Of Equations"
        << "\n\t3 . Matrices Oparetors"
        << "\n\t0 . Exit" << endl;
    cin >> option;

    switch ( option ) {
        case '1': {
            /* Arrays Algebra Functions */
            int row = 1, col = 1;
            cout << "input [lines digit]: ";   cin >> row;
            cout << "input [columns digit]: "; cin >> col;

            register Array < double > *MyArray = new Array < double > (row, col);
            MyArray-> read();

            cout << "The Array :\n";
            MyArray-> display();

            if (! (MyArray->Row() - MyArray->Col())) {
                cout << "The Determinant(Array) = " << MyArray-> determinant() << endl;

                if (MyArray-> determinant()) {
                    cout << "The Inverse :\n";
                    MyArray-> invers().display();
                } else cout << "There are no inverse \n";
            }
            break;
        }
        case '2': {
            /* Solving a System of Equations */

            int row = 1, col = 1;
            cout << "input [Equations digit]: "; cin >> row;
            cout << "input [Variables digit]: "; cin >> col;

            register Array < double > *system = new Array < double >(row, col + 1);

            system-> readEquation();
            system-> writeEquation();

            register Array < double > solve(InvertSolutions(* system));//solve(col, 1);
            char constant = 't';
            cout << "Solutions:" << endl;
            for (int i = 0; i < system-> Col() - 1; i++)
                cout << constant++ << "  =  " << solve[i][0] << endl;
            break;
        }
        case '3': {
            cout << "Input elements of the [first] matrix ..\n";
            int row = 1, col = 1;
            cout << "input A[lines digit]: ";   cin >> row;
            cout << "input A[columns digit]: "; cin >> col;
            register Array < double > A(row, col);
            A.read();

            cout << "\nInput elements of the [second] matrix ..\n";
            cout << "input B[lines digit]: ";   cin >> row;
            cout << "input B[columns digit]: "; cin >> col;

            register Array < double > B(row, col);
            B.read();

            cout << "Array [A]:\n";
            A.display();

            cout << "Array [B]:\n";
            B.display();

            // Multiplication
            cout << "Multiplication A * B\n";
            if ( !(A.Col() - B.Row())) (A * B).display();
            else cout << "can't do Multiplication operator";
            cout << '\n';

            break;
        }
    }

    return option == '0' ? 0 : main(argc, argv);
}

template <class type>  inline Array < type > InvertSolutions(Array < type > MainEquations) {

    Array < type > Oprands(MainEquations.Row(), MainEquations.Col() - 1);

    for (int i = 0; i < Oprands.Row(); i++)
        for (int j = 0; j < Oprands.Col(); j++)
            Oprands[i][j] = MainEquations[i][j];

    double det = Oprands.determinant();

    if ( det ) {
        Array < type > Left(MainEquations.Row(), 1);
        for (int i = 0; i < MainEquations.Row(); i++)
            Left[i][0] = MainEquations[i][MainEquations.Col() - 1];

        return Oprands.invers() * Left;
    }
}
