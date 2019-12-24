use std::fmt;

struct Matrix {
    values: [f64; 4],
}

impl Matrix {
    fn new(a: f64, b: f64, c: f64, d: f64) -> Matrix {
        Matrix {
            values: [a, b, c, d]
        }
    }

    fn determinant(&self) -> f64 {
        self.values[0]*self.values[3] - self.values[1]*self.values[2]
    }
    
    fn transpose(&mut self) {
        self.values.swap(2, 3);
    }
    
    fn inverse(&self) -> Option<Matrix> {
        let det = self.determinant();
        if det == 0. {
            None
        }
        else {
            Some(Matrix::new(self.values[3]/det, -self.values[1]/det, 
                            -self.values[2]/det,  self.values[0]/det))
        }
    }
}

impl fmt::Display for Matrix {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "[ {:.2}, {:.2} ; {:.2}, {:.2} ]", self.values[0], 
            self.values[1], self.values[2], self.values[3])
    }

}

fn main()
{
    let m = Matrix::new(2., 3., -2., 1.);

    println!("A = {}", m);
    println!("det(A) = {}", m.determinant());
    
    let mut m = m;
    m.transpose();
    
    println!("A^T = {}", m);
    println!("A^-1 = {}", m.inverse().unwrap());
    
    let m = Matrix::new(4., 8., 3., 6.);
    println!("B = {}", m);
    if let Some(x) = m.inverse() {
        println!("B^-1 = {}", x);
    }
    else {
        println!("B is not invertible");
    }
}
