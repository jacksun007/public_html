use std::ops::Add;

struct Fibonacci<T> {
    curr: T,
    next: T,
}

impl<T: Copy + Add<Output=T>> Iterator for Fibonacci<T> {
    type Item = T;
    
    fn next(&mut self) -> Option<T> {
        let new_next = self.curr + self.next;
        self.curr = self.next;
        self.next = new_next;
        Some(self.curr)
    }
}

fn fibonacci() -> Fibonacci<u32> {
    Fibonacci { curr: 0, next: 1 }
}

fn main() {
	for i in fibonacci().skip(3).take(4) {
		println!("{}", i);
	}
}
