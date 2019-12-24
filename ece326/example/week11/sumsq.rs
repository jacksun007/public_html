use std::default::Default;
use std::ops::{AddAssign, Mul};

// Default: trait to come up with a default value for the type (e.g. 0 for int)
// AddAssign: trait to do +=
// Mul<Output=T>: trait to do *, on top of that, Output=T means the result
// of the multiplication needs to be of the same type (i.e. typeof(T*T) == T)

fn sum_of_squares<T: Default + AddAssign + Mul<Output=T> + Copy>(list: &[T]) -> T
{
    let mut sum = Default::default();
    
    for v in list {
        sum += *v * *v;
    }
    
    return sum;
}


fn main() {
    let iarray = [1, 5, 6, 2, 3];
    let farray = [3.2, 1.6, 7.5, 0.2, -3.4];

    println!("{:?} => {}", iarray, sum_of_squares(&iarray));
    println!("{:?} => {}", farray, sum_of_squares(&farray));
}