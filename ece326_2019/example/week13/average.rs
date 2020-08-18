use std::f64::consts::PI;

trait Shape {
    fn area(&self) -> f64;
}

struct Triangle {
    base: f64,
    height: f64,      
}

impl Shape for Triangle {
    fn area(&self) -> f64 {
        self.base * self.height / 2.
    }
}

struct Rectangle {
    width: f64,
    height: f64,
}

impl Shape for Rectangle {
    fn area(&self) -> f64 {
        self.width * self.height
    }
}

struct Circle {
    radius: f64,
}

impl Shape for Circle {
    fn area(&self) -> f64 {
        PI * self.radius
    }
}

fn average_area(list: Vec<Box<dyn Shape>>) -> f64 {
    if list.len() > 0 {
        (list.len() as f64).powf(-1.) *
        list.into_iter().fold(0., |ac, shape| ac + shape.area())
    } 
    else {
        0.
    }
}

/* this macro takes one or more expressions and adds all the expressions
 * into a list of Box so I don't haev to repeat Box::new for each object */
macro_rules! boxvec {
    ($($x:expr),+) => {
        vec![ $(Box::new($x)),+ ]
    }
}

fn main() {
    let v: Vec<Box<dyn Shape>> = boxvec![ 
        Triangle { base: 3., height: 7. },
        Triangle { base: 4.5, height: 2. },
        Rectangle { width: 4., height: 6. },        
        Circle { radius: 5. }
    ];

    println!("average area: {}", average_area(v));
}
