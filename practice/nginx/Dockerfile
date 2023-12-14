FROM nginx:latest

RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y vim
RUN mkdir -p /usr/share/nginx/html/test
RUN chmod +x /usr/share/nginx/html/test

COPY default.conf /etc/nginx/conf.d/default.conf
COPY index.html /usr/share/nginx/html/test/index.html


EXPOSE 80
EXPOSE 8080

CMD ["nginx", "-g", "daemon off;"]
