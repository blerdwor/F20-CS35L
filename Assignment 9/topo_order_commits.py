# Keep the function signature,
# but replace its body with your implementation.
#
# Note that this is the driver function.
# Please write a well-structured implemention by creating other functions outside of this one,
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables that are modifiable.
# This is because those variables live on forever once the script is imported,
# and the changes to them will persist across different invocations of the imported functions.
import os
import sys
import zlib

# main function
def topo_order_commits():
   # first find the git directory and get its path
   git_path = get_git_path() + "/.git"

   # next, go into the refs/heads to obtain the branch names
   # key = the commit hash that the branch stores, value = branch name
   branch_dict = get_branches(git_path)
   #for branch in branch_dict:
   #   print(branch, branch_dict[branch])
   #print('\n')
   branch_hashes = list(branch_dict.keys())

   # next, perform a DFS on every branch in the dictionary
   # turns all the commits into nodes, and gets root nodes for topo order
   commit_nodes, root_hashes = build_commit_graph(git_path, branch_hashes)
   # for c in commit_nodes:
   #   print(commit_nodes[c].commit_hash, "Parent: ", commit_nodes[c].parents)
   # print("----------------------------------------------")
   # print(root_hashes)
   # print('\n')
   # topo sort, so fun YAY
   sorted_commits = get_topo_ordered_commits(commit_nodes, root_hashes)
  
   # printing!!!
   print_topo_ordered_commits_with_branch_names(commit_nodes, sorted_commits, branch_dict)

# class definition
class CommitNode:
   def __init__(self, commit_hash):
      """
      :type commit_hash: str
      """
      self.commit_hash = commit_hash
      self.parents = set()
      self.children = set()

# print function
def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
   jumped = False
   for i in range(len(topo_ordered_commits)):
      commit_hash = topo_ordered_commits[i]
      if jumped:
         jumped = False
         sticky_hash = ' '.join(commit_nodes[commit_hash].children)
         print(f'={sticky_hash}')
      branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
      print(commit_hash + (' '+' '.join(branches) if branches else'')) 
      if i+1<len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
         jumped = True
         sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
         print(f'{sticky_hash}=\n')
 
# perform topological sort on the commits, starting with the root hashes
def get_topo_ordered_commits(commit_nodes, root_hashes):
   order = []
   visited = set()
   gray_stack = []
   stack=sorted(root_hashes) # list

   while stack:
      v = stack.pop() # v is a hash of a root node

      if v in visited: # skip processed nodes
         continue
      
      visited.add(v) # process node
 
      while gray_stack and v not in commit_nodes[gray_stack[-1]].children: # is not a child of temp_stack.top()
         g = gray_stack.pop()
         order.append(g)
      
      gray_stack.append(v)

      for c in sorted(commit_nodes[v].children):
         if c not in visited:
            stack.append(c)

   while gray_stack: # add remaining nodes to order
      order.append(gray_stack.pop())

   return order  

# perform DFS on every branch from step 2
# Trace through the parents using the information from decompressed object files in the .git/objects folder
def build_commit_graph(git_dir, local_branch_heads):  
   commit_nodes = {}
   root_hashes = set()
   visited = set()
   stack = sorted(local_branch_heads) # list

   while stack:
      commit_hash = stack.pop()

      if commit_hash in visited: # skip processed nodes
         continue

      visited.add(commit_hash) # process the node

      if commit_hash not in commit_nodes: # make commit into node, it it doesn't exist
         commit_node = CommitNode(commit_hash)
         commit_nodes[commit_hash] = commit_node

      # get the entire node + parents   
      commit_node = commit_nodes[commit_hash]
      hash_file = git_dir + "/objects/" + commit_hash[:2] + '/' + commit_hash[2:]
      cc = open(hash_file, 'rb').read()
      dc = zlib.decompress(cc).decode().split('\n')
      for d in dc:
         if d[:6]=="parent":
            commit_node.parents.add(d[7:])

      if not commit_node.parents: # means this is a root node
         root_hashes.add(commit_hash)

      for p in commit_node.parents: # make parent, store child hash in parent
         if p not in visited:
            stack.append(p)
         if p not in commit_nodes:
            commit_nodes[p] = CommitNode(p)
         commit_nodes[p].children.add(commit_hash)
      
   return commit_nodes, root_hashes

# find all the branches as returns them as a dictionary
def get_branches(git_path):
   heads = git_path+"/refs/heads/"
   branch_dict = {}
   branches_list = []

   #get list of branch names from .git/refs/heads
   for root, dirs, files in os.walk(heads):
      for name in files:
         path = os.path.join(root, name)
         if path[:len(heads)]==heads:
            # remove path of heads from full path
            path = path[len(heads):]
         branches_list.append(path)

   #get hashes for each branch and add it to a hash table
   for branch in branches_list:
      branch_hash=open(os.path.join(heads,branch), 'r').read().strip()
      if branch_hash not in branch_dict.keys():
         branch_dict[branch_hash] = []
      branch_dict[branch_hash].append(branch)

   return branch_dict

# finds the git directory if there exists one and returns the path
def get_git_path():
   # get current directory that python script is called in
   curr_dir = os.curdir

   #infinite loop until .git is found
   while True:
      # error if we've reached the root node
      if os.path.abspath(curr_dir)=="/":
         sys.exit("Not inside a Git repository")
    
      # goes through the files and checks if any are .git
      # if git is not found, then we move up a directory level
      for d in os.listdir(curr_dir):
         if d == ".git" and os.path.isdir(os.path.join(curr_dir,d)):
            return os.path.abspath(curr_dir)
      curr_dir = "../" + curr_dir

if __name__ == '__main__':
   topo_order_commits()
