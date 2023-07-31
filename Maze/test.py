class S:
    
    def __init__(self) -> None:
        pass

    def __f(self,x, n):
        return ((-1)**n)*self.__f2(x,n)
    def __f2(self,x,n):
      tich=1
      for i in range(1,2*n+1):
        tich=tich*x/i
      if n == 0:
         return 1
      return tich
    def caculate(self, x):
      tong=0
      for i in range (0,10000) :
        tong+=self.__f(x,i)
      return tong
x=float(input())
tinh_s = S()
print("Giá trị của S là:", tinh_s.caculate(x))