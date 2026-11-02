# Cloud Records Microservice Container (Task 12 Adaptation)
FROM debian:bookworm-slim
WORKDIR /app
COPY edge_cloud_sim.c /app/
RUN apt-get update && apt-get install -y gcc libc6-dev && gcc -std=c11 -O2 edge_cloud_sim.c -o edge_cloud_sim
CMD ["./edge_cloud_sim"]
