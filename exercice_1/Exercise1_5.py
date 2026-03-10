from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# Value sent by each process
send_value = 1000 + rank

# Define neighbors in the ring
next_rank = (rank + 1) % size
prev_rank = (rank - 1 + size) % size

# Send then receive
comm.send(send_value, dest=next_rank)
received_value = comm.recv(source=prev_rank)

print(f"Process {rank} received {received_value} from process {prev_rank}")