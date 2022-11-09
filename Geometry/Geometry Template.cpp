#include <bits/stdc++.h>

#define ll  long long
#define ld long double
#define el "\n"
using namespace std;
typedef complex<ld> point;
#define X real()
#define Y imag()
#define angle(a) atan2((a).imag(), (a).real())
#define vec(a, b) ((b) - (a))
#define length(a) (hypot((a).imag(), (a).real()))
// dp = a * b * cos(T) if 0 -> perp
#define dot(a, b) ((conj(a) * (b)).real())
// cp = a * b * sin(T) if 0 -> parallel
#define cross(a, b) ((conj(a) * (b)).imag())
#define lengthSqr(v) (dot(v, v))
#define normalize(a) ((a) / length(a))
#define same(a, b) (lengthSqr(vec(a, b)) < EPS)
#define rotate(v, t) (polar(v, t))
#define rotate0(p, ang) ((p) * exp(point(0, ang))
#define rotateabout(p, ang, about) (rotate0(vec(about, p), ang) + about)
#define reflect(p, m) ((conj((p) / (m))) * (m))
const ld PI = acos(-1.0), EPS = 1e-8;
ld toDegreeFromMinutes(ld minutes){
    return minutes / 60;
}
ld toRadians(ld degree){
    return degree * PI / 180.0;
}
ld toDegree(ld radian){
    if(radian < 0) radian += 2 * PI;
    return radian * 180 / PI;
}
ld fixAngle(ld A){
    return A > 1 ? 1 : (A < -1 ? -1 : A);
}
// sin(A) / a = sin(B) / b = sin(C) / c
ld getSide_a_bAB(ld b, ld A, ld B) {
    return sin(A) * b / sin(B);
}
ld getAngle_A_abB(ld a, ld b, ld B) {
    return asin(fixAngle((a * sin(B)) / b));
}
// a^2 = b^2 + c^2 - 2 * b * c * cos(A)
ld getAngle_A_abc(ld a, ld b, ld c) {
    return acos(fixAngle((b * b + c * c - a * a) / (2 * b * c)));
}
int dcmp(ld a, ld b){
    return fabs(a - b) <= EPS ? 0 : a < b ? -1 : 1;
}
point reflect2(point p, point p0, point p1){
    point z = p - p0, w = p1 - p0;
    return conj(z / w) * w + p0;  // Refelect point p around p0p1
}
bool isCollinear(point a, point b, point c){ // point c between a and b
    return fabs(cross(b - a, c - a)) < EPS;
}
bool isPointOnRay(point a, point b, point c) {
    if (!isCollinear(a, b, c))
        return false;
    return dcmp(dot(b - a, c - a), 0) == 1;
}
bool isPointOnSegment(point a, point b, point c) {
    double acb = length(a - b), ac = length(a - c), cb = length(b - c);
    return dcmp(acb - (ac + cb), 0) == 0;
}
//  distance between point c and line ab perpendicular
ld distToLine(point a, point b, point c){
    ld dist = cross(b - a, c - a) / length(a - b);
    return fabs(dist);
}
// distance from point p2 to segment p0-p1
ld distToSegment(point p0, point p1, point p2){
    point v1 = p1 - p0, v2 = p2 - p0;
    ld d1 = dot(v1, v2), d2 = dot(v1, v1);
    // left
    if(d1 <= 0)
        return length(p2 - p0);
    // right
    if(d2 <= d1)
        return length(p2 - p1);
    // on segment
    ld t = d1 / d2;
    return length(p2 - (p0 + v1 * t));
}
bool intersectSegments(point a, point b, point c, point d, point & intersect) {
    ld d1 = cross(a - b, d - c), d2 = cross(a - c, d - c), d3 = cross(a - b, a - c);
    if (fabs(d1) < EPS)
        return false;  // Parllel || identical

    ld t1 = d2 / d1, t2 = d3 / d1;
    intersect = a + (b - a) * t1;

    if (t1 < -EPS || t2 < -EPS || t2 > 1 + EPS)
        return false;  //e.g ab is ray, cd is segment ... change to whatever
    return true;
}
// Where is P2 relative to segment p0-p1?
// ccw = +1 => angle > 0 or collinear after p1
// cw = -1 => angle < 0 or collinear after p0
// Undefined = 0 => Collinar in range [a, b]. Be careful here
int ccw(point a, point b, point c) {
    point v1(b - a), v2(c - a);
    ld t = cross(v1, v2);

    if (t > +EPS)
        return 1;
    if (t < -EPS)
        return -1;
    if (v1.X * v2.X < -EPS || v1.Y * v2.Y < -EPS)
        return -1;
    if (norm(v1) < norm(v2) - EPS)
        return 1;
    return 0;
}
bool intersect(point p1, point p2, point p3, point p4) {
    // special case handling if a segment is just a point
    bool x = (p1 == p2), y = (p3 == p4);
    if (x && y) return p1 == p3;
    if (x) return ccw(p3, p4, p1) == 0;
    if (y) return ccw(p1, p2, p3) == 0;

    return ccw(p1, p2, p3) * ccw(p1, p2, p4) <= 0 &&
           ccw(p3, p4, p1) * ccw(p3, p4, p2) <= 0;
}
ld triangleArea(point p0, point p1, point p2){
    ld a = length(p0 - p1), b = length(p0 - p2), c = length(p1 - p2);
    ld s = (a + b + c) / 2;
    return sqrt((s - a) * (s - b) * (s - c) * s); // Heron's formula
    /*
      base = u + v (divided by h) u = ((a ^ 2) + (b ^ 2) - (c ^ 2)) / (2 * a)
      h = sqrt(b ^ 2 - u ^ 2) where base is a

      If three point are on circle boundary (Triangle inside Circle)
      ld radius = (a * b * c) / (4 * triangleArea)

      If Circle inside Triangle
      ld radius = sqrt((s - a) * (s - b) * (s - c) / s)
     */
}
// Given the length of three medians of a triangle
ld triangleArea2(ld m1, ld m2, ld m3){
    // Area of triangle using medians as sides
    // 3 / 4 * (area of original triangle)
    if(m1 <= 0 || m2 <= 0 || m3 <= 0) return -1; // impossible
    ld s = 0.5 * (m1 + m2 + m3);
    ld medians_area = (s * (s - m1) * (s - m2) * (s - m3));
    ld area = 4.0 / 3.0 * sqrt(medians_area);
    if(medians_area <= 0.0 || area <= 0) return -1; // impossible
    return area;
}
// 2 points has infinite circles
// Find circle passes with 3 points, some times, there is no circle! (in case colinear)
// Draw two perpendicular lines and intersect them
pair<ld, point> findCircle(point a, point b, point c) {
    //create median, vector, its prependicular
    point m1 = (b + a) * (ld)0.5, v1 = b - a, pv1 = point(v1.Y, -v1.X);
    point m2 = (b + c) * (ld)0.5, v2 = b - c, pv2 = point(v2.Y, -v2.X);
    point end1 = m1 + pv1, end2 = m2 + pv2, center;
    intersectSegments(m1, end1, m2, end2, center);
    return make_pair(length(a - center), center);
}

// If line intersect cirlce at point p, and p = p0 + t(p1-p0)
// Then (p-c)(p-c) = r^2 substitute p and rearrange
// (p1-p0)(p1-p0)t^2 + 2(p1-p0)(p0-C)t + (p0-C)(p0-C) = r*r; -> Quadratic
vector<point> intersectLineCircle(point p0, point p1, point C, double r) {
    ld a = dot(p1 - p0, p1 - p0), b = 2 * dot(p1 - p0, p0 - C),
            c = dot(p0 - C, p0 - C) - r * r;
    ld f = b * b - 4 * a * c;

    vector<point> v;
    if (dcmp(f, 0) >= 0) {
        if (dcmp(f, 0) == 0) f = 0;
        ld t1 = (-b + sqrt(f)) / (2 * a);
        ld t2 = (-b - sqrt(f)) / (2 * a);
        v.push_back(p0 + t1 * (p1 - p0));
        if (dcmp(f, 0) != 0) v.push_back(p0 + t2 * (p1 - p0));
    }
    return v;
}
vector<point> intersectCircleCircle(point c1, ld r1, point c2, ld r2) {
    // Handle infinity case first: same center/radius and r > 0
    if (same(c1, c2) && dcmp(r1, r2) == 0 && dcmp(r1, 0) > 0)
        return {3, c1};    // infinity 2 same circles (not points)

    // Compute 2 intersection case and handle 0, 1, 2 cases
    ld ang1 = angle(c2 - c1), ang2 = getAngle_A_abc(r2, r1, length(c2 - c1));

    if (::isnan(ang2)) // if r1 or d = 0 => nan in getAngle_A_abc (/0)
        ang2 = 0; // fix corruption

    vector<point> v(1, polar(r1, ang1 + ang2) + c1);

    // if point NOT on the 2 circles = no intersection
    if (dcmp(dot(v[0] - c1, v[0] - c1), r1 * r1) != 0 ||
        dcmp(dot(v[0] - c2, v[0] - c2), r2 * r2) != 0)
        return {};

    v.push_back(polar(r1, ang1 - ang2) + c1);
    if (same(v[0], v[1]))  // if same, then 1 intersection only
        v.pop_back();
    return v;
}

const int MAX = 100000 + 9;
point pnts[MAX], r[3], cen;
double rad;
int ps, rs;	// ps = n, rs = 0, initially

// Pre condition
// random_shuffle(pnts, pnts+ps);		rs = 0;
void MEC() {
    if (ps == 0 && rs == 2) {
        cen = (r[0] + r[1]) / (ld)2.0;
        rad = length(r[0] - cen);
    } else if (rs == 3) {
        pair<ld, point> p = findCircle(r[0], r[1], r[2]);
        cen = p.second;
        rad = p.first;
    } else if (ps == 0) {
        cen = r[0];    // sometime be garbage, but will not affect
        rad = 0;
    } else {
        ps--;
        MEC();
        if (length(pnts[ps] - cen) > rad) {
            r[rs++] = pnts[ps];
            MEC();
            rs--;
        }
        ps++;
    }
}

ld polygonArea(vector<point>& points) {
    ld a = 0;
    for (int i = 0; i < points.size(); i++)
        a += cross(points[i], points[(i + 1) % points.size()]);
    return fabs(a / 2.0);    // If area > 0 then points ordered ccw
}
// 2 means on polygon
// 1 means in polygon
// 0 means out of polygon
int isInsidePoly(vector<point> p, point p0) {
    int wn = 0;  // the winding number counter

    for (int i = 0; i < p.size(); i++) {
        point cur = p[i], nxt = p[(i + 1) % p.size()];
        if (isPointOnSegment(cur, nxt, p0))
            return 2; // on polygon
        if (cur.Y <= p0.Y) {    // Upward edge
            if (nxt.Y > p0.Y && cross(nxt - cur, p0 - cur) > EPS)
                ++wn;
        } else {                // Downward edge
            if (nxt.Y <= p0.Y && cross(nxt - cur, p0 - cur) < -EPS)
                --wn;
        }
    }
    return wn != 0;
}
// assume p0 is added again to p (n+1 points)
bool isSimplePolygon(vector<point> &p) {
    for (int i = 0; i < p.size() - 1; i++) {
        for (int j = i + 2; j < p.size() - 1; j++)
            if (intersect(p[i], p[i + 1], p[j], p[j + 1]) &&
                (i != 0 || j != p.size() - 2) /* last/first edges are consecutive*/)
                return false;   //Segments must not share vertices
    }
    return true;
}

bool isConvexPolygon(vector<point> &p) {
    // all polygon 3 consecutive points must have same sign (ccw or cw)
    p.push_back(p[0]), p.push_back(p[1]); // wrap points for simplicity

    int sign = ccw(p[0], p[1], p[2]);
    bool ok = true;
    for (int i = 1; ok && i < p.size() - 2; i++) {
        if (ccw(p[i], p[i + 1], p[i + 2]) != sign)
            ok = false;
    }
    p.pop_back(), p.pop_back();
    return ok;
}

// isConcave = !isConvex
// isConcave = 2 different signs

/*
 * Note : Pick's Theorem
 * for Simple polygon -> Area(P) = internal points + (boundary points / 2) - 1
 * To get the number of boundary points from (a, b) to (c, d)
 * we compute gcd(c - a, d - b) + 1
 */

point polygonCenteriod(vector<point> points) {
    ld x = 0, y = 0, a = 0, c;

    for (int i = 0; i < points.size(); ++i) {
        int j = (i + 1) % (int)points.size();
        c = cross(points[i], points[j]), a += c;
        x += (points[i].X + points[j].X) * c;
        y += (points[i].Y + points[j].Y) * c;
    }
    if (dcmp(a, 0) == 0)
        return (points[0] + points.back()) * (ld)0.5;   // Line
    a /= 2, x /= 6 * a, y /= 6 * a;

    // Fix values in case
    if (dcmp(x, 0) == 0) x = 0;
    if (dcmp(y, 0) == 0) y = 0;

    return {x, y};
}

// P need to be counterclockwise convex polygon
pair<vector<point>, vector<point> > polygonCut(vector<point> &p, point A, point B) {

    vector<point> left, right;
    point intersect;

    for (int i = 0; i < p.size(); ++i) {
        point cur = p[i], nxt = p[(i + 1) % (int) p.size()];

        if (cross(B - A, cur - A) >= 0)
            right.push_back(cur);

        //NOTE adjust intersectSegments should handled AB as line
        if (intersectSegments(A, B, cur, nxt, intersect)) {
            right.push_back(intersect);
            left.push_back(intersect);
        }

        if (cross(B - A, cur - A) <= 0)
            left.push_back(cur);
    }
    return make_pair(left, right);
}
ld distSq(point p, point q)
{
    return (p.real() - q.real()) * (p.real() - q.real()) + (p.imag() - q.imag()) * (p.imag() - q.imag());
}

bool isSquare(vector<point>v)
{
    ld d2 = distSq(v[0], v[1]);
    ld d3 = distSq(v[0], v[2]);
    ld d4 = distSq(v[0], v[3]);

    if (d2 < EPS || d3 < EPS || d4 < EPS)
        return false;


    if (fabs(d2 - d3) < EPS && fabs(2 * d2 - d4) < EPS
        && fabs(2 * distSq(v[1], v[3]) - distSq(v[1], v[2])) < EPS) {
        return true;
    }

    if (fabs(d3 - d4) < EPS && fabs(2 * d3 - d2) < EPS
        && fabs(2 * distSq(v[2], v[1]) - distSq(v[2], v[3])) < EPS) {
        return true;
    }
    if (fabs(d2 - d4) < EPS && fabs(2 * d2 - d3) < EPS
        && fabs(2 * distSq(v[1], v[2]) - distSq(v[1], v[3])) < EPS) {
        return true;
    }
    return false;
}
int main() {
    complex<ld>num1(2, 3);
    cout << num1.real() << " " << num1.imag() << el; // 2 + 3i
    complex<ld>num2(1, 1);
    cout << "Modulus " << abs(num2) << el; // r
    cout << "Argument " << arg(num2) << el; // radian
    cout << "Angle " << arg(num2) * 180 / acos(-1.0) << el; // degree
    cout << "Norm " << norm(num2) << el; // (1 * 1 + 1 * 1)
    complex<ld>num3 = polar(abs(num2), arg(num2)); // r and theta
    cout << num3 << el;
    complex<ld> i = -1;
    cout << sqrt(i) << el;
    complex<ld> c(2, 3);
    cout << conj(c) << el;
    cout << pow(c, 2) << el;
    complex<ld> i1(0, -1);
    cout << exp(i1 * PI) << el;
    return 0;
}