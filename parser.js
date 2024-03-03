const fs  = require("node:fs");

const filename = "./reqdata.dat";


fs.readFile(filename,"utf8",(err,reqdata)=>{
  if(err) return -1;
  const reqbuf = reqdata.split("\n");
  let fbuf = reqbuf[0].replace("\r","").split(" ");
  process.stdout.write(fbuf[0].trim()+"\n")
  if(fbuf[1].includes("?")){
    const [route,search] = fbuf[1].split("?");
    process.stdout.write(route.trim()+"\n"+search.trim()+"\n");
  }else {
    process.stdout.write(fbuf[1].trim()+"\n\n");
  }

  fbuf = reqbuf.filter((h,i)=>h.includes("Host:")|| h.includes("host:"));
  let host = fbuf[0].split(" ")[1].trim().replace("\r","");
  process.stdout.write(host+"\n");
  if(host.includes(":")){
    const [hostname,port] = host.split(":");
    process.stdout.write(hostname+"\n"+port+"\n");
  }else {
    process.stdout.write(host+"\n\n");
  }
  fbuf = reqbuf.filter((h,i)=>h.includes("Accept:")|| h.includes("accept:"));
  let accept = fbuf[0].split(": ")[1].trim().replace("\r","");
  process.stdout.write(accept+"\n");
  fbuf = reqbuf.filter((h,i)=>h.includes("User-Agent:")||h.includes("user-agent:"));
  let user_agent = fbuf[0].split(": ")[1].trim().replace("\r","");
  process.stdout.write(user_agent+"\n");
  return 0;
})
