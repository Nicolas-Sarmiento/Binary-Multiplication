#include <iostream>
#include <vector>
#include <bitset>
#include <climits>
#include <cmath>
#define N 16 // N bits constant
using namespace std;
using ll = long long;


/*
    Struct to represent 2D points.
*/
struct Point {
    ll x;
    ll y;
};

/*
    Gauss-Jordan solver, returns a coefficient array.
*/
vector<long double> gauss(vector<vector<long double>> A, vector<long double> b) {
    int n = (int)A.size();
    for (int i = 0; i < n; i++) {
        A[i].push_back(b[i]);
    }

    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(A[j][i]) > abs(A[pivot][i]))
                pivot = j;
        }
        swap(A[i], A[pivot]);

        long double div = A[i][i];
        if (abs(div) < 1e-18) continue; 
        for (int j = i; j <= n; j++) A[i][j] /= div;

        for (int j = 0; j < n; j++) {
            if (j == i) continue;
            long double factor = A[j][i];
            for (int k = i; k <= n; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }

    vector<long double> x(n);
    for (int i = 0; i < n; i++) x[i] = A[i][n];
    return x;
}

/*
    Interpolation function that returns a coefficent array of interpoled Function.
    Convert points into a matrix to solve using gauss-jordan method.
*/  
vector<ll> interpolate(const vector<Point>& pts) {
    int n = (int)pts.size();
    vector<vector<long double>> A(n, vector<long double>(n, 0));
    vector<long double> b(n);

    for (int i = 0; i < n; i++) {
        ll xi = pts[i].x;
        b[i] = pts[i].y;
        if (xi == LLONG_MAX) { 
            A[i][n-1] = 1.0;
        } else {
            long double p = 1;
            for (int j = 0; j < n; j++) {
                A[i][j] = p;
                p *= (long double)xi;
            }
        }
    }

    vector<long double> sol = gauss(A, b);

    vector<ll> coeffs(n);
    for (int i = 0; i < n; i++) {
        coeffs[i] = llround(sol[i]);
    }
    return coeffs;
}

/*
    Print the polynomial from of a function by its coeeficient array.
*/
void print_polynomial(vector<ll> &coeff_vector){
    if ( coeff_vector.size() < 1) return;
    cout << coeff_vector[0];
    for( ll i = 1; i < coeff_vector.size(); i++){
        cout << " + " << coeff_vector[i] << "x^" << i;
    }
}

/*
    Power method using fast exponentation 
*/

ll powll(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;  
        base *= base;
        exp >>= 1; 
    }
    return result;
}

/*
    Evals a polynomial in an x point.
*/

ll evaluate_polynomial(vector<ll> &coeff_vector, ll x){
    if ( coeff_vector.size() < 1 ) return 0;
    ll val = 0;
    for(ll i = 0; i < coeff_vector.size(); i++ ){
        val += coeff_vector[i] * (powll(x, i));
    }
    return val;
}


int main() {
    string num_1, num_2;
    ll k ;
    cout << "Input number A in radix 2: ";
    cin >> num_1;
    cout << "Input number B in radix 2: ";
    cin >> num_2;
    cout <<"Input k (3 <= k <= 16): ";
    cin >> k;

    if ( num_1.length() > 16 || num_2.length() > 16 || (k > 16 || k < 3)  ){
        cout << "Invalid args!!\n";
        return 0;
    }

    ll a, b, coeff_a, coeff_b, bit_mask, m, eval_x;

    try {
        a = std::stoll(num_1, nullptr, 2);  
    }
    catch (const std::invalid_argument& e) {
        cerr << "Invalid number for A.\n";
        return 0;
    }

    try {
        b = std::stoll(num_2, nullptr, 2);  
    }
    catch (const std::invalid_argument& e) {
        cerr << "Invalid number for B.\n";
        return 0;
    }


    cout << "A radix 10: " << a << " B radix 10: " << b << '\n';
    cout << "Converting now into polynomial form:\n";

    m = (N / k) + (N % k ? 1 : 0);
    bit_mask = (1 << m) - 1;
    
    vector<ll> Ax, Bx;
    
    for( ll i = 0; i < k; i++){
        coeff_a = (a & bit_mask) >> (m*i);
        coeff_b = (b & bit_mask) >> (m*i);

        Ax.push_back(coeff_a);
        Bx.push_back(coeff_b);

        bit_mask <<= m;

    }

    cout << "Ax = ";
    print_polynomial(Ax);
    cout << '\n';
    cout << "Bx = ";
    print_polynomial(Bx);
    cout << '\n';

    cout << "Evaluation points: \n";

    vector<Point> product_points;
    product_points.push_back({LLONG_MAX, Ax.back() * Bx.back()});
    eval_x = k - 1;

    for(ll i = 0; i < (2*k - 2); i++){
        ll product = evaluate_polynomial(Ax, eval_x) * evaluate_polynomial(Bx, eval_x);
        product_points.push_back({eval_x, product});
        eval_x--;
    }

    for(Point p : product_points){
        cout << "x : ";
        if ( p.x == LLONG_MAX ) cout << "INF ";
        else cout << p.x;
        cout  << " y: " << p.y << '\n';
    }

    
    cout << "Interpolating points: \n";
    vector<ll> Zx = interpolate(product_points);

    cout << "Z(x) = ";
    print_polynomial(Zx);
    cout << '\n';


    ll multiplication = evaluate_polynomial(Zx, (1<<m));
    cout << "AB = " <<bitset<32>(multiplication)  << " Radix 2, " << multiplication << " Radix 10 \n"; 

}