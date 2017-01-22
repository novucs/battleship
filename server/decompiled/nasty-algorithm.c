
if (this.speedX[index] == 0 & this.speedX[index] == 0)
{
  this.speedX[index] = (int)Math.Round((double)Conversion.Int(unchecked(10f * VBMath.Rnd() + 1f))) - 5;
  this.speedY[index] = (int)Math.Round((double)Conversion.Int(unchecked(10f * VBMath.Rnd() + 1f))) - 5;
}
if (this.shipX[index] > 990)
{
  this.speedX[index] = -2;
}
if (this.shipX[index] < 10)
{
  this.speedX[index] = 2;
}
if (this.shipY[index] > 990)
{
  this.speedY[index] = -2;
}
if (this.shipY[index] < 10)
{
  this.speedY[index] = 2;
}
int num = -1;
int num2 = 1000;
int num3 = 0;
do
{
  if ((this.inuse[num3] & this.active[num3]) && num3 != index && Operators.ConditionalCompareObjectLess(this.Distance[index, num3], this.VisibleRange(index, num3), false) && num2 > this.Distance[index, num3])
  {
    num2 = this.Distance[index, num3];
    num = num3;
  }
  num3++;
} while (num3 <= 200);

if (num >= 0 & num <= 200)
{
  if (this.fireCount[index] < 10)
  {
    this.fire[index] = true;
    this.fireX[index] = this.shipX[num];
    this.fireY[index] = this.shipY[num];
    this.fireCount[index] = this.fireCount[index] + 1;
  }
  else
  {
    this.fireCount[index] = 1;
  }
  if (this.shipX[index] < this.shipX[num])
  {
    this.speedX[index] = 2;
  }
  if (this.shipX[index] > this.shipX[num])
  {
    this.speedX[index] = -2;
  }
  if (this.shipY[index] < this.shipY[num])
  {
    this.speedY[index] = 2;
  }
  if (this.shipY[index] > this.shipY[num])
  {
    this.speedY[index] = -2;
  }
  this.flag[index] = this.flag[num];
}
this.moves[index] = true;
