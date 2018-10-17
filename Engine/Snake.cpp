#include "Snake.h"
#include <assert.h>
Snake::Snake(const Location & loc) 
{
	seg[0].InitHead(loc,Colors::White);
}
void Snake::Move(const Location & dloc)
{
	Location m = { 0,0 };
	if (dloc != m) {
		for (int i = nseg - 1; i > 0; --i) {
			seg[i].Follow(seg[i - 1]);
		}
		seg[0].Move(dloc);
	}
}
void Snake::Grow(Color c)
{
	seg[nseg].InitBody(c);
	if (nseg < maxseg) ++nseg;
}
void Snake::Draw(Board & brd) const 
{
	
	seg[0].Draw(brd);
	for (int i = 1; i < nseg; i++) {
		seg[i].Draw(brd);
	}
}
Location Snake::getnextloc(const Location & dloc) const
{
	Location l(seg[0].getloc());
	l.Add(dloc);
	return l;
}
bool Snake::IsInTileExceptend(const Location & target) const
{
	for (int i = 0; i < nseg-1; ++i) {
		if (target != seg[0].getloc()) {
			if (seg[i].getloc() == target) {
				return true;
			}
		}
	}
	return false;
}
bool Snake::IsInTile(const Location & target) const
{
	for (int i = 0; i < nseg; ++i) {
		if (seg[i].getloc() == target) {
			return true;
		}
	}
	return false;
}
const Location& Snake::Segment::getloc() const
{
	return loc;
}
void Snake::Segment::InitHead(const Location & in_loc,Color col) {
	loc = in_loc;
	c = col;
}
void Snake::Segment::InitBody(Color col)
{
	c = col;
}
void Snake::Segment::Move(const Location & dloc)
{
	assert( ( (abs(dloc.x) + abs(dloc.y) ) == 1) || ( (abs(dloc.x) + abs(dloc.y)) == 0));
	loc.Add(dloc);
}
void Snake::Segment::Follow(const Segment & next)
{
	loc = next.loc;
}
void Snake::Segment::Draw(Board & brd) const{
	brd.drawcellthick(loc,c,thick);
}
void Snake::resetsnake(Board &brd)
{
	nseg = 1;
	seg[0].loc = { brd.Getwidth() / 2, brd.Getheight() / 2 };
}
