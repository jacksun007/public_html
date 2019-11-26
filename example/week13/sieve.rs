fn main() {
    let starter: Vec<i32> = vec![2, 3, 5, 7];
    let largest = 50;
    let composites = starter.iter().map(|&x| -> Vec<i32> { 
            (1..largest).filter(|&y| y % x == 0).collect() 
        }).flatten().collect::<Vec<i32>>();
    let primes : Vec<i32> = (1..largest).filter(|&x| !composites.contains(&x))
        .collect();
    println!("{:?}", primes);
}
