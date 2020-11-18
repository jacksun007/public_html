use std::time::{SystemTime, UNIX_EPOCH};
use std::fmt;
use std::ops::{Index, IndexMut};
use std::thread;
use std::sync::Arc;
use std::sync::mpsc::channel;

fn rand(start: i32, end: i32) -> i32 {
    let nanos = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .subsec_nanos();
    start + (nanos as i32 % (end - start))
}

struct Matrix {
    mat: Vec<Vec<i32>>,
}

impl Matrix {
    fn new(dim: usize) -> Matrix {
        let mut ret = Matrix {
            mat: vec![],    
        };
    
        for _ in 0..dim {
            let mut v = vec![];
            for _ in 0..dim {
                v.push(rand(-5, 9));
            }
            ret.mat.push(v);
        }
        
        ret
    }
}

impl fmt::Display for Matrix {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {  
        for row in &self.mat {
            for col in row {
                write!(f, "{:4} ", col)?;
            }
            write!(f, "\n")?;
        }
        Ok(())
    }
}

impl Index<usize> for Matrix {
    type Output = Vec<i32>;

    fn index(&self, idx: usize) -> &Self::Output {
        &self.mat[idx]
    }
}

impl IndexMut<usize> for Matrix {
    fn index_mut(&mut self, idx: usize) -> &mut Self::Output {
        &mut self.mat[idx]
    }
}

fn main() {
    let dim = 20;
    let vals = Arc::new((Matrix::new(dim), Matrix::new(dim)));
    
    println!("m = [\n{}]", vals.0);
    println!("n = [\n{}]", vals.1);
    
    /* stores channels for receiving answers */
    let mut ans = vec![];
 
    let mut ret = Matrix { mat: vec![], };
    
    for t in 0..dim {
        let (sender, receiver) = channel();
        let vals = vals.clone();

        thread::spawn(move || {
            let mut row = vec![];
            let (m, n) = &*vals;
            
            for i in 0..dim {
                let mut sum = 0;
                for j in 0..dim {
                    sum += m[t][j] * n[j][i];
                }
                row.push(sum);
            }
        
            sender.send(row).unwrap();
        });

        ans.push(receiver);
    }
    
    for r in ans {
        ret.mat.push(r.recv().unwrap());
    }
    
    println!("ret = [\n{}]", ret);
}
