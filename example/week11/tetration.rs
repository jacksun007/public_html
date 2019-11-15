// TryInto Trait attempts conversion, and returns error on failure
// It is safer than using as operator since TryInto will catch overflow
use std::convert::TryInto;

trait Tetration {
    // Self is the type of self (the struct implementing the trait)
    fn tetration(&self, n: i32) -> Self;
}

impl Tetration for i64 {
    fn tetration(&self, n: i32) -> i64 {
        if n == 0 {
            1
        }
        else {
            // pow is a method for i64 which implements exponentiation
            self.pow(self.tetration(n-1).try_into().unwrap())
        }
    }
}

fn main()
{
    let n = 3;
    let i: i64 = 3;
    let v = i.tetration(n);
    
    // tet(3, 3) = 7625597484987
    println!("tet({}, {}) = {}", i, n, v);
}
