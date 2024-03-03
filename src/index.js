
console.log("RUNNING JAVASCRIPT");

let deg=0;
const figures = document.querySelectorAll("figure");
figures.forEach((elem,i)=>{
  elem.addEventListener("click",(evt)=>{
    //console.log(evt);
    evt.target.style.transform = `rotate(${deg+5}deg)`;
    deg+=10;
  })
});
