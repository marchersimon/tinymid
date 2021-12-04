# ICL Specification

This page documents the structure of an ICL file and how to properly docode it. It is currently at version **1**.

## Structure

<table>
<tr><td>"ICL"</td><td>3 Bytes</td></tr>
<tr><td>Version number</td><td>2 Bytes</td></tr>
<tr><td>max. number of notes</td><td>1 Byte</td></tr>
<tr><td>Song name</td><td></td></tr>
<tr><td>Instructions</td><td></td></tr>
</table>

Every ICL file starts with the literal ASCII characters "ICL" followed by the version number consisting of 2 bytes. The next byte represents the maximal number of notes playing at the same time appearing in the file. Following, the name of the song is specified as null-terminated string of ASCII characters, however it can also be empty. Lastly, there are all instructions to be interpreted.

## Instructions

<table>
<tr><td>Delta time</td><td>1..4 Bytes</td></tr>
<tr><td>Instruction type</td><td>1 Bytes</td></tr>
<tr><td>Data</td><td></td></tr>

</table>

The **delta time** is a variable length value as defined in the standard MIDI file. If the MSB of a byte is 1 it means another byte follows. Then, the remaining 7 bits of each byte represent the actual delta time. Here is a code example on how that could be done:

```C
uint32_t deltaTime = 0;
for(int i = 0; i < 4; i++) {
    uint8_t byte = getNextByte();
    deltaTime = (value << 7) | (byte & 0x7F);
    if(~byte & 0x80) {
        break;
    }
}
```

Each **instruction type** has a specific amount of data following. It can be one of the following:

<table>
<tr><th>Event</th><th>Description</th></tr>
<tr><td>0000vvvv 0kkkkkkk</td><td>Note on</br>vvvv represents the velocity (volume) of the note</br>kkkkkkk represents the note (key) number<sup><a href="#table-of-note-numbers">[1]</sup></td></tr>
<tr><td>0001vvvv 0kkkkkkk</td><td>Note off</br>vvvv represents the velocity (sustain) of the note</br>kkkkkkk represents the note (key) number<sup><a href="#table-of-note-numbers">[1]</sup></td></tr>
<tr><td>00100000 tempo</br></td><td>Set tempo</br>3 byte tempo copied from the midi file</td></tr>
<tr><td>11110000</br></td><td>End of file</br></td></tr>
</table>

## Table of note numbers

<table>
<tbody><tr><th>Octave #</th><th colspan="12">Note Numbers</th></tr>
<tr><th>&nbsp;</th><th>C</th><th>C#</th><th>D</th><th>D#</th><th>E</th><th>F</th>
<th>F#</th><th>G</th><th>G#</th><th>A</th><th>A#</th><th>B</th></tr>
<tr><td>-1</td><td>0</td><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>6</td><td>7</td><td>8</td><td>9</td><td>10</td><td>11</td></tr>
<tr><td>0</td><td>12</td><td>13</td><td>14</td><td>15</td><td>16</td><td>17</td><td>18</td><td>19</td><td>20</td><td>21</td><td>22</td><td>23</td></tr>
<tr><td>1</td><td>24</td><td>25</td><td>26</td><td>27</td><td>28</td><td>29</td><td>30</td><td>31</td><td>32</td><td>33</td><td>34</td><td>35</td></tr>
<tr><td>2</td><td>36</td><td>37</td><td>38</td><td>39</td><td>40</td><td>41</td><td>42</td><td>43</td><td>44</td><td>45</td><td>46</td><td>47</td></tr>
<tr><td>3</td><td>48</td><td>49</td><td>50</td><td>51</td><td>52</td><td>53</td><td>54</td><td>55</td><td>56</td><td>57</td><td>58</td><td>59</td></tr>
<tr><td>4</td><td>60</td><td>61</td><td>62</td><td>63</td><td>64</td><td>65</td><td>66</td><td>67</td><td>68</td><td>69</td><td>70</td><td>71</td></tr>
<tr><td>5</td><td>72</td><td>73</td><td>74</td><td>75</td><td>76</td><td>77</td><td>78</td><td>79</td><td>80</td><td>81</td><td>82</td><td>83</td></tr>
<tr><td>6</td><td>84</td><td>85</td><td>86</td><td>87</td><td>88</td><td>89</td><td>90</td><td>91</td><td>92</td><td>93</td><td>94</td><td>95</td></tr>
<tr><td>7</td><td>96</td><td>97</td><td>98</td><td>99</td><td>100</td><td>101</td><td>102</td><td>103</td><td>104</td><td>105</td><td>106</td><td>107</td></tr>
<tr><td>8</td><td>108</td><td>109</td><td>110</td><td>111</td><td>112</td><td>113</td><td>114</td><td>115</td><td>116</td><td>117</td><td>118</td><td>119</td></tr>
<tr><td>9</td><td>120</td><td>121</td><td>122</td><td>123</td><td>124</td><td>125</td><td>126</td><td>127</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
</tbody></table>
