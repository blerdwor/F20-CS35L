#!/usr/bin/env python3

"""
Shuffle input before redirecting to output
"""

import random, sys, argparse

class shuf:
   def __init__(self,iList,count,repeat):
      self.List=iList
      self.count=count
      self.repeat=repeat

   def shuffle(self):
      random.shuffle(self.List)

      # if -r was specified
      if self.repeat:
         if self.count>0:
            while self.count>0:
               print(random.choice(self.List))
               self.count-=1
         else:
            while True:
               for elem in self.List:
                  print(elem)
               random.shuffle(self.List)

      # if -r was not specified, but -n was
      elif self.count>0:
         if self.count>len(self.List):
            self.count=len(self.List)

         for i in range(0,self.count):
             print(self.List[i])

      else:
         for elem in self.List:
            print(elem)

# main function, i.e., starting point in Python
def main():

   # add flags
   parser = argparse.ArgumentParser()

   parser.add_argument("-i","--input-range",action="store",type=str,dest="irange",default="",
   			help="Input two numbers in the format LO-HI where LO is a number less than HI")
   parser.add_argument("-n","--head-count",action="store",dest="count",type=int,default=0,
			help="Outputs up to COUNT numbers")
   parser.add_argument("-r","--repeat", action="store_true",dest="repeat",default=False,
			help="Program repeats until user terminates the program")
   parser.add_argument("filename",nargs="?")

   args=parser.parse_args()
   # print(args)
   argCount=len(sys.argv)
   iList=[]
   
   # check if -i and file were inputted
   if args.filename and args.irange!="":
      sys.exit("Error: both input-range and file were provided")

   # check -n flag
   if args.count<0:
      sys.exit("Invalid count: {}".format(count))

   # -i flag was specified
   if args.irange:
      try:
         num1,num2=args.irange.split("-")
         if num1.isnumeric() and num2.isnumeric():
            num1=int(num1)
            num2=int(num2)
         else:
            sys.exit("Invalid input range: {}".format(args.irange))
      except:
         sys.exit("Invalid input range: {}".format(args.irange))
		
      if num1>num2 or num1<0:
         sys.exit("Invalid input range: LO must be a postiive number less than HI")

      iList=[num for num in range(num1,num2+1)]

   # no file or just a "-" was specified
   elif args.filename=="-" or not args.filename:
         for line in sys.stdin:
            iList.append(line.rstrip('\n'))

   # file was specified
   else:

      # read from file
      try:
         with open(args.filename) as filename:
            for line in filename.readlines():
               iList.append(line.rstrip('\n'))
      except:
         sys.exit("Error: Invalid file")

   magic=shuf(iList,args.count,args.repeat)
   magic.shuffle()

if __name__=="__main__":
	main()
