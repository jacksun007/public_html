#[derive(Debug,Clone,Copy)]
struct Point {
    x: f32,
    y: f32,
    z: f32
}

let p = Point{x: 1., y: 2., z: 3. };
let q = p.clone();
let r = p;