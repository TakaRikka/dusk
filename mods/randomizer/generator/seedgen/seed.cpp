#include "seed.hpp"

#include "../utility/random.hpp"

#include <string>

namespace randomizer::seedgen::seed
{
    static constexpr const char* nouns[] = {
        "Aeralfos",  "Agitha",    "Ant",        "Argorok",    "Armos",      "Ashei",      "Auru",     "BackSlice", "Bari",
        "Barnes",    "Beamos",    "Beth",       "BigBaba",    "Blizzeta",   "Bo",         "Bokoblin", "Bombfish",  "Borville",
        "Bulblin",   "Butterfly", "CastleTown", "Charlo",     "Cheese",     "Chilfos",    "Chu",      "Chudley",   "Clawshot",
        "Colin",     "Coro",      "Cucco",      "Dangoro",    "Darbus",     "Darkhammer", "Darknut",  "Dayfly",    "DeathSword",
        "DekuToad",  "Dodongo",   "Dragonfly",  "Dynalfos",   "Eldin",      "Epona",      "Fado",     "Fairy",     "Falbi",
        "Fanadi",    "Faron",     "Freezard",   "Fyer",       "Ganondorf",  "Gengle",     "GhoulRat", "Gibdo",     "Goron",
        "GreatSpin", "Greengill", "Guay",       "Hanch",      "Hawkeye",    "Helmasaur",  "Hena",     "Hornet",    "HorseGrass",
        "Hylian",    "Jaggle",    "Jovani",     "JumpStrike", "Keese",      "Kili",       "Ladybug",  "Lanayru",   "Lantern",
        "Leever",    "Link",      "Lizalfos",   "Louise",     "Luda",       "Malo",       "Malver",   "Mantis",    "Midna",
        "Misha",     "Moldorm",   "Morpheel",   "Ooccoo",     "Ordona",     "Pergie",     "Phasmid",  "Plumm",     "Poe",
        "Postman",   "Pumpkin",   "Puppet",     "Purdy",      "Ralis",      "Reekfish",   "Renado",   "Rupee",     "Rusl",
        "Rutela",    "Sage",      "Sera",       "Shad",       "ShellBlade", "Sketch",     "SkullKid", "Skulltula", "SkyBook",
        "Snail",     "Snowpeak",  "Soal",       "Soldier",    "Spinner",    "Stalfos",    "Stallord", "Talo",      "Tektite",
        "Telma",     "Temple",    "TileWorm",   "Toadpoli",   "Trill",      "Twilight",   "Uli",      "WolfLink",  "Zant",
        "Zelda",     "Zora"
    };

    static constexpr const char* adjectives[] = {
        "Abnormal",   "Absent",     "Absolute",   "Abstract",   "Absurd",     "Accurate",   "Active",     "Actual",
        "Adjacent",   "Aesthetic",  "Aggressive", "Alert",      "Alien",      "Alternate",  "Amazing",    "Ambitious",
        "Amusing",    "Ancient",    "Angry",      "Anxious",    "Apparent",   "Artistic",   "Astute",     "Atomic",
        "Atrocious",  "Attractive", "Authentic",  "Average",    "Awful",      "Awkward",    "Bad",        "Bashful",
        "Basic",      "Beautiful",  "Big",        "Bitter",     "Bizarre",    "Blue",       "Bold",       "Brainy",
        "Brave",      "Bright",     "Brilliant",  "Busy",       "Callous",    "Calm",       "Capable",    "Careful",
        "Casual",     "Cautious",   "Central",    "Cheap",      "Cheerful",   "Chemical",   "Chilly",     "Chronic",
        "Chummy",     "Circular",   "Civil",      "Classic",    "Clean",      "Clever",     "Clinical",   "Clumsy",
        "Coastal",    "Cognitive",  "Coherent",   "Cold",       "Colorful",   "Comical",    "Commercial", "Common",
        "Compact",    "Competent",  "Complete",   "Complex",    "Concise",    "Concrete",   "Confident",  "Confused",
        "Consistent", "Constant",   "Contrary",   "Cool",       "Corny",      "Corporate",  "Correct",    "Cosmic",
        "Costly",     "Courteous",  "Cranky",     "Crazy",      "Creative",   "Credible",   "Creepy",     "Criminal",
        "Critical",   "Curious",    "Current",    "Custom",     "Cute",       "Daily",      "Damp",       "Dangerous",
        "Dapper",     "Dark",       "Deadly",     "Decent",     "Decisive",   "Defeated",   "Defensive",  "Defiant",
        "Delicate",   "Delightful", "Desperate",  "Detached",   "Determined", "Different",  "Difficult",  "Digital",
        "Diligent",   "Disastrous", "Disgusted",  "Distant",    "Disturbed",  "Divine",     "Dizzy",      "Dominant",
        "Double",     "Doubtful",   "Dramatic",   "Dreadful",   "Droll",      "Dull",       "Dynamic",    "Early",
        "Effective",  "Elated",     "Elderly",    "Electric",   "Elegant",    "Empty",      "Endless",    "Enormous",
        "Entire",     "Equal",      "Essential",  "Eternal",    "Evil",       "Excellent",  "Exotic",     "Expensive",
        "Explicit",   "Extreme",    "Factual",    "Faithful",   "False",      "Famous",     "Fancy",      "Fantastic",
        "Fast",       "Fatal",      "Favorite",   "Fellow",     "Fierce",     "Final",      "Financial",  "Foolish",
        "Formal",     "Fortified",  "Fortunate",  "Frantic",    "Free",       "Frenzied",   "Fresh",      "Friendly",
        "Functional", "Funny",      "Furious",    "Future",     "Galactic",   "Generous",   "Genial",     "Gentle",
        "Genuine",    "Giant",      "Glad",       "Glass",      "Glittery",   "Gloomy",     "Glorious",   "Golden",
        "Good",       "Gothic",     "Graceful",   "Gradual",    "Grand",      "Great",      "Grim",       "Gross",
        "Grumpy",     "Guilty",     "Handsome",   "Happy",      "Harmful",    "Harsh",      "Healthy",    "Hearty",
        "Heavy",      "Helpful",    "Historic",   "Honest",     "Hostile",    "Huge",       "Hungry",     "Hyper",
        "Impartial",  "Implicit",   "Important",  "Impressive", "Indirect",   "Indoor",     "Infinite",   "Inherent",
        "Initial",    "Inner",      "Innocent",   "Inspiring",  "Instant",    "Intense",    "Internal",   "Inventive",
        "Jarring",    "Jealous",    "Jolly",      "Joyful",     "Junior",     "Kind",       "Kooky",      "Late",
        "Lazy",       "Lesser",     "Lethargic",  "Light",      "Likable",    "Linear",     "Linguistic", "Liquid",
        "Little",     "Lively",     "Local",      "Logical",    "Lonely",     "Loud",       "Lovely",     "Loyal",
        "Lucky",      "Lunar",      "Mad",        "Magical",    "Magnetic",   "Mainstream", "Majestic",   "Major",
        "Malicious",  "Manual",     "Marine",     "Marvellous", "Massive",    "Maximum",    "Mean",       "Meaningful",
        "Medical",    "Medieval",   "Medium",     "Mellow",     "Mental",     "Mere",       "Middle",     "Mighty",
        "Mild",       "Minimal",    "Mobile",     "Modest",     "Monthly",    "Moral",      "Motionless", "Muddy",
        "Mundane",    "Musical",    "Mutual",     "Nasty",      "Natural",    "Nearby",     "Neat",       "Negative",
        "Nerdy",      "Nervous",    "Neutral",    "Nice",       "Nimble",     "Noble",      "Noisy",      "Notable",
        "Objective",  "Obnoxious",  "Obscure",    "Obvious",    "Odd",        "Offensive",  "Official",   "Okay",
        "Old",        "Only",       "Opposite",   "Optical",    "Optional",   "Organic",    "Organized",  "Outdoor",
        "Painful",    "Paper",      "Parallel",   "Past",       "Patient",    "Peaceful",   "Peppy",      "Perfect",
        "Permanent",  "Persistent", "Personal",   "Petty",      "Pink",       "Plain",      "Platinum",   "Plausible",
        "Pleasant",   "Polite",     "Popular",    "Portable",   "Positive",   "Potential",  "Powerful",   "Practical",
        "Precious",   "Pretty",     "Previous",   "Primitive",  "Private",    "Probable",   "Productive", "Profound",
        "Prominent",  "Proper",     "Protective", "Public",     "Pure",       "Purple",     "Purposeful", "Puzzled",
        "Quick",      "Quiet",      "Quirky",     "Random",     "Rapid",      "Rational",   "Recent",     "Redundant",
        "Refined",    "Regretful",  "Regular",    "Relaxed",    "Relevant",   "Remote",     "Reserved",   "Resident",
        "Responsive", "Retail",     "Rigid",      "Rival",      "Romantic",   "Rotten",     "Royal",      "Rubber",
        "Rude",       "Sacred",     "Sad",        "Safe",       "Scary",      "Seasonal",   "Secret",     "Secured",
        "Selective",  "Senior",     "Sensible",   "Serious",    "Severe",     "Shady",      "Shallow",    "Sharp",
        "Sheer",      "Shiny",      "Short",      "Sick",       "Sideways",   "Silent",     "Silly",      "Silver",
        "Similar",    "Simple",     "Sincere",    "Skilled",    "Skittish",   "Sleepy",     "Slow",       "Small",
        "Smart",      "Smug",       "Snazzy",     "Snooty",     "Solar",      "Solid",      "Somber",     "Spare",
        "Specific",   "Spiteful",   "Splendid",   "Spooky",     "Spotless",   "Spry",       "Square",     "Stable",
        "Standard",   "Startled",   "Static",     "Steady",     "Stern",      "Stone",      "Stylish",    "Subsequent",
        "Successful", "Sudden",     "Suitable",   "Sunny",      "Super",      "Supportive", "Surplus",    "Suspicious",
        "Sweet",      "Symbolic",   "Talkative",  "Tall",       "Tearful",    "Technical",  "Terrible",   "Thankful",
        "Thoughtful", "Thrilled",   "Tidy",       "Tired",      "Total",      "Tough",      "Toxic",      "Tragic",
        "Tremendous", "Trivial",    "Tropical",   "Troubled",   "Truthful",   "Typical",    "Ultimate",   "Ultra",
        "Unaware",    "Uncertain",  "Unfair",     "Unforeseen", "Uniform",    "Unique",     "Unknown",    "Unlawful",
        "Unlikely",   "Unreal",     "Upbeat",     "Upset",      "Urban",      "Useful",     "Usual",      "Vague",
        "Valid",      "Verbal",     "Vertical",   "Vicious",    "Vigorous",   "Villainous", "Virtual",    "Visible",
        "Vital",      "Vivid",      "Warm",       "Weekly",     "Weird",      "Wholesome",  "Wicked",     "Wise",
        "Wistful",    "Witty",      "Wonderful",  "Wooden",     "Worried",    "Wrong",      "Young",      "Zany"
    };

    int GetRandValue(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        std::random_device rd;
        std::mt19937 engine(rd());
        return distribution(engine);
    }

    std::string GenerateSeed()
    {
        const std::string adjective1 = adjectives[GetRandValue(0, std::size(adjectives) - 1)];
        const std::string adjective2 = adjectives[GetRandValue(0, std::size(adjectives) - 1)];
        const std::string noun = nouns[GetRandValue(0, std::size(nouns) - 1)];

        return adjective1 + adjective2 + noun;
    }

    std::string GenerateHash()
    {
        const std::string noun1 = utility::random::RandomElement(nouns);
        const std::string noun2 = utility::random::RandomElement(nouns);
        const std::string noun3 = utility::random::RandomElement(nouns);

        return noun1 + " " + noun2 + " " + noun3;
    }
} // namespace randomizer::seedgen::seed
