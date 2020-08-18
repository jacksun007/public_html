def main():
    numbers = [2, 3, 5, 7]
    it = iter(numbers)
    while True:
        try:
            print(next(it))
        except StopIteration:
            print("End of List")
            break
            
if __name__ == "__main__":
    main()